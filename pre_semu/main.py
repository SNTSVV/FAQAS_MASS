#! /usr/bin/env python

"""
Implement meta-mu source generator
TODO: Implement it as following:
    - Modify MASS so that when generating each mutant file, 
        another file is generated, following the Meta-mu format
        but having that single mutant (and only original in switch).
        * Use same tmp variable id accross mutants (keep a map between node and tmp id)
        * The whole code that changes accross mutants on the same statement (around the switch part)
           Must be surrounded by special comments, or just do the diff of mutants on same line to figure out the change
    - In this module, implements some code to unify the generated single mutant meta-mu files
"""

import os
import argparse
import collections
import json
import re
from itertools import groupby
from operator import itemgetter

#import difflib
import pandas as pd

META_MU_INFO_FILE_NAME = "meta-mu.info"

StmtInfo = collections.namedtuple("StmtInfo", ["start_index", "end_index"])
ChangedInfo = collections.namedtuple("ChangedInfo", ["start_index", "end_index", "filename", "mut_str"]) #End index character is excluded

def is_case_stmt(stmt_start_pos, code_str):
    sub_code = code_str[stmt_start_pos:]
    if re.match('^case(\s|//|/\*)', sub_code):
        return True
    return False

def is_switch_cond(stmt_start_pos, code_str):
    sub_code = code_str[stmt_start_pos:]
    if re.match('^switch(\(|\s|//|/\*)', sub_code):
        return True
    return False  


def process_case_mutation(changed_list, original_src_file, sorted_stmt_list):
    case_to_switch = {}
    switches = []
    with open(original_src_file) as f:
        orig_str = f.read()
    for stmt in sorted_stmt_list:
        if is_case_stmt(stmt.start_index, orig_str):
            # Find containing switch
            for i in range( len(switches) - 1, -1, -1) :
                sw = switches[i]
                if sw.start_index <= stmt.start_index and sw.end_index >= stmt.end_index:
                    case_to_switch[stmt] = sw
                    break
            assert stmt in case_to_switch, "case statement without corresponding switch ({})".format(stmt)
        elif is_switch_cond(stmt.start_index, orig_str):
            switches.append(stmt)
        else:
            continue

    new_changed_list = []
    for ci in changed_list:
        for case, sw in case_to_switch.items():
            if case.start_index <= ci.start_index and case.end_index >= ci.end_index:
                # mutate the whole switch
                new_start_index = sw.start_index
                new_end_index = sw.end_index
                new_mut_str = orig_str[new_start_index: ci.start_index] + ci.mut_str + orig_str[ci.end_index:new_end_index]
                ci = ChangedInfo(
                    start_index=new_start_index, 
                    end_index=new_end_index, 
                    filename=ci.filename, 
                    mut_str=new_mut_str
                )
                break
        new_changed_list.append(ci)
    return new_changed_list

class MutantInfo:
    def __init__(self, stmt_info, changed_info, int_id):
        self.raw_id = self._computer_raw_id(changed_info.filename)
        self.int_id = int_id
        self.stmt_info = stmt_info
        self.changed_info = changed_info

    def _is_whole_stmt(self):
        return self.stmt_info.start_index == self.changed_info.start_index \
                and self.stmt_info.end_index == self.changed_info.end_index

    def _computer_raw_id(self, mut_filename):
        return os.path.basename(mut_filename)

    @classmethod
    def get_mutant_list (cls, original_src_file, mutant_src_file_list, meta_mu_info_file, no_skip_non_function_mutants=False):
        stmt_list = cls._get_stmt_list(original_src_file, meta_mu_info_file)
        mut_list = []
        changed_list = cls._get_changed_list (original_src_file, mutant_src_file_list, meta_mu_info_file)
        changed_list = process_case_mutation(changed_list, original_src_file, stmt_list)
        changed_list.sort(key=lambda x: (x.start_index, x.end_index))
        stmt_i = 0
        for int_idx, ci in enumerate(changed_list):
            stmt_info = None
            while stmt_i < len(stmt_list):
                if ci.start_index < stmt_list[stmt_i].start_index:
                    if no_skip_non_function_mutants:
                        assert False, "mutant not in any stmt ({})".format(ci.filename)
                    else:
                        break
                if stmt_i + 1 < len(stmt_list):
                    next_stmt = stmt_list[stmt_i + 1]
                    if next_stmt.start_index <= ci.start_index:
                        # stmt nesting, use next stmt
                        stmt_i += 1
                        continue
                if ci.start_index < stmt_list[stmt_i].end_index:
                    assert ci.end_index <= stmt_list[stmt_i].end_index, \
                                                "mutants spawn multiple stmts ({})".format(ci.filename)
                    stmt_info = stmt_list[stmt_i]
                    break
                stmt_i += 1
            if stmt_info is None:
                if no_skip_non_function_mutants:
                    assert False, "Could not find stmt for mutant {}".format(ci.filename)
                else:
                    continue

            mut_list.append(
                MutantInfo(
                    stmt_info=stmt_info,
                    changed_info=ci,
                    int_id=int_idx+1
                )
            )
        

        return mut_list

    @staticmethod
    def get_change_key_to_start_and_afterend(meta_mu_info_file, orig_lines_idx):
        meta_mu_info_df = pd.read_csv(meta_mu_info_file)
        assert set(meta_mu_info_df) == {"MUTANT_FILE", "START_LINE", "START_COL", "SIZE"}, "Invalid meta mu info"
        mut_info_map = {}
        for row in meta_mu_info_df.itertuples():
            row = row._asdict()
            fn = os.path.basename(row["MUTANT_FILE"])
            assert fn not in mut_info_map, "duplicate mutant basename in meta mu info ()".format(meta_mu_info_file)
            mut_info_map[fn] = {k: row[k] for k in ("START_LINE", "START_COL", "SIZE")}

        fn_to_start_afterend = {}
        for fn, mut_info in mut_info_map.items():
            try:
                mut_idx = orig_lines_idx[mut_info["START_LINE"]] + mut_info["START_COL"] - 1
            except IndexError as e:
                print("\n# mut_info['START_LINE'] is", mut_info["START_LINE"], ", len(orig_lines_idx) is", len(orig_lines_idx), "\n")
                raise
            mut_after_end = mut_idx + mut_info["SIZE"]
            fn_to_start_afterend[fn] = (mut_idx, mut_after_end)
        return fn_to_start_afterend

    @staticmethod
    def load_original_data(original_src_file):
        #seq_matcher = SequenceMatcher()
        with open(original_src_file) as f:
            orig_str = f.read()
            f.seek(0)
            orig_lines_idx = [None, 0]
            for l in f.readlines():
                orig_lines_idx.append(orig_lines_idx[-1] + len(l))
            del orig_lines_idx[-1]
            #print('DBG', len(orig_lines_idx)-1, orig_lines_idx[-1], len(l), len(orig_str), l)
        return orig_str, orig_lines_idx

    @staticmethod
    def _get_stmt_list (original_src_file, meta_mu_info_file):
        """
        take original and SDL mutants, anf find stmts
        """
        orig_str, orig_lines_idx = MutantInfo.load_original_data(original_src_file)
        fn_to_start_afterend = MutantInfo.get_change_key_to_start_and_afterend(meta_mu_info_file, orig_lines_idx)

        stmt_list = []
        for fn, (start_index, post_end_index) in fn_to_start_afterend.items():
            # if nt SDL, discard
            dot_split = fn.split(".")
            if "SDL" != dot_split[-3]:
                continue

            stmt_list.append(
                StmtInfo(
                    start_index=start_index,
                    end_index=post_end_index
                )
            )
        assert len(stmt_list) > 0, "No stmt found"
        stmt_list.sort(key=lambda x: (x.start_index, x.end_index))

        return stmt_list

    @staticmethod
    def _get_changed_list (original_src_file, mutant_src_file_list, meta_mu_info_file):
        # ensure no basename duplicate
        assert len(set([os.path.basename(fn) for fn in mutant_src_file_list])) == len(mutant_src_file_list), "duplicate mutant basename"

        orig_str, orig_lines_idx = MutantInfo.load_original_data(original_src_file)

        fn_to_start_afterend = MutantInfo.get_change_key_to_start_and_afterend(meta_mu_info_file, orig_lines_idx)

        #seq_matcher.set_seq1(orig_str)
        changed_list = []
        for mutant_src_file in mutant_src_file_list:
            mutant_basename = os.path.basename(mutant_src_file)
            mut_idx, mut_after_end = fn_to_start_afterend[mutant_basename]

            with open(mutant_src_file) as f:
                mutant_str = f.read()

            mut_chunk_end = len(mutant_str) - len(orig_str[mut_after_end:])
            assert mut_chunk_end >= 0, "problem with mutant"
            assert mutant_str[mut_chunk_end:] == orig_str[mut_after_end:], "suffix mismatch"

            mut_chunk_str = mutant_str[mut_idx: mut_chunk_end]
            changed_list.append(
                ChangedInfo(
                    start_index=mut_idx,
                    end_index=mut_after_end, 
                    filename=mutant_src_file, 
                    mut_str=mut_chunk_str
                )
            )
        return changed_list
        
def get_stmt_to_expr_to_mutant(mutant_list):
    """
    Take the mutant list, group by expression and statement, alterating mutant info accordingly
    """
    stmt_to_expr_to_mut = {}
    
    stmt_to_muts = {}
    for mut_info in mutant_list:
        if mut_info.stmt_info not in stmt_to_muts:
            stmt_to_muts[mut_info.stmt_info] = []
            stmt_to_expr_to_mut[mut_info.stmt_info] = {}
        stmt_to_muts[mut_info.stmt_info].append(mut_info)
        expr = (mut_info.changed_info.start_index, mut_info.changed_info.end_index)
        if expr not in stmt_to_expr_to_mut[mut_info.stmt_info]:
            stmt_to_expr_to_mut[mut_info.stmt_info][expr] = []
        stmt_to_expr_to_mut[mut_info.stmt_info][expr].append(mut_info)
        
        def merge_crossing(range_to_vlist):
            crossing = True
            while crossing:
                crossing = False
                tmp = list(range_to_vlist)
                for r_idx in range(len(tmp)):
                    if crossing:
                        break
                    for o_idx in range(r_idx+1, len(tmp)):
                        r_dat = tmp[r_idx]
                        o_dat = tmp[o_idx]
                        if min(r_dat[1], o_dat[1]) - max(r_dat[0], o_dat[0]) >= 0 \
                                    and (r_dat[1] - o_dat[1]) * (r_dat[0] - o_dat[0]) > 0:
                            # there is crossing, merge
                            merged_obj = range_to_vlist[r_dat] + range_to_vlist[o_dat]
                            del range_to_vlist[r_dat]
                            del range_to_vlist[o_dat]
                            new_expr = min(r_dat[0], o_dat[0], max(r_dat[1], o_dat[1]))
                            if new_expr in range_to_vlist:
                                range_to_vlist[new_expr] += merged_obj
                            else:
                                range_to_vlist[new_expr] = merged_obj
                            crossing = True
                            assert False, "Got crossing, strange. (TODO: expand the mutant replacement code and uncomment this) {}, {}".format(r_dat, o_dat)
                            break
                
        # get expressions
        for stmt, expr_data in stmt_to_expr_to_mut.items():
            merge_crossing(expr_data)
            
    return stmt_to_expr_to_mut

def apply_metamu(stmt_to_expr_to_mut, orig_src_str):
    """
    take stmt to expr to mutant and the original source code string and return the meta-mutant source code
    """
    # rank the statements by decreasing start and increasing end
    stmts = sorted(list(stmt_to_expr_to_mut), reverse=True, key=lambda x:(x.start_index, -x.end_index))
    
    code = {}
    for i in range(len(orig_src_str)):
        code[i] = {(i, i+1): orig_src_str[i]}
    #print(len(orig_src_str) + 1)

    def get_code(start, after_end):
        result = ""
        tmp_start = start
        tmp_after_end = after_end
        while True:
            # get the larget interval starting at tmp_start
            largest = max(code[tmp_start].keys(), key=lambda x:(int(tmp_after_end>=x[1]), x[1]-tmp_after_end))
            result += code[tmp_start][largest]
            if largest[1] < tmp_after_end:
                #print("DBG", tmp_start, largest[1], tmp_after_end, code[largest[1]])
                tmp_start = largest[1]
            else:
                break;
        return result

    def set_code(start, after_end, code_str):
        code[start][(start, after_end)] = code_str
            
    stmt2mutant_ids = {}
    for stmt in stmts:
        stmt2mutant_ids[stmt] = []
        exprs = sorted(list(stmt_to_expr_to_mut[stmt]), reverse=True, key=lambda x:(x[0], -x[1]))
        for expr in exprs:
            existing_code = get_code(expr[0], expr[1])
            id2repl = {mut.int_id: mut.changed_info.mut_str for mut in stmt_to_expr_to_mut[stmt][expr]}
            #print("# DBG", id2repl, stmt_to_expr_to_mut[stmt][expr][0].changed_info.mut_str, stmt_to_expr_to_mut[stmt][expr][0].changed_info.filename)
            if stmt_to_expr_to_mut[stmt][expr][0]._is_whole_stmt():
                mutated = compute_switch_stmt(id2repl, existing_code)
            else:
                mutated = compute_selection_expr(existing_code, id2repl)
            set_code(expr[0], expr[1], mutated)
            stmt2mutant_ids[stmt] += list(id2repl)
    for stmt, mut_list in stmt2mutant_ids.items():
        existing_code = get_code(stmt.start_index, stmt.end_index)
        sel_stmts = compute_mutation_points(mut_list)
        set_code(stmt.start_index, stmt.end_index, sel_stmts + existing_code)
    return get_code(0, len(orig_src_str))

MID_SEL_GLOBAL = "klee_semu_GenMu_Mutant_ID_Selector"

def compute_mutation_points(mut_list):
    s_list = sorted(mut_list)
    ranges =[]

    for k,g in groupby(enumerate(s_list),lambda x:x[0]-x[1]):
        group = (map(itemgetter(1),g))
        group = list(map(int,group))
        ranges.append((group[0],group[-1]))

    sel = "\n"
    for id1, id2 in ranges:
        sel += compute_mutation_point_str(id1, id2) + "\n"
    return sel

def compute_mutation_point_str(mut_start_id, mut_end_id):
    return "klee_semu_GenMu_Mutant_ID_Selector_Func({},{});".format(mut_start_id, mut_end_id)

def compute_selection_expr(orig_expr, mid2expr):
    res = "(" + orig_expr + ")"
    for mid, mexpr in sorted(mid2expr.items()):
        res = "({}=={}?\n({}):\n{})".format(MID_SEL_GLOBAL, mid, mexpr, res)
    return res

def compute_switch_stmt(id2stmt, default_stmt):
    res = "switch (klee_semu_GenMu_Mutant_ID_Selector) {\n"
    for mid, stmt in id2stmt.items():
        res += "    case {}: {{{}{} break;}}\n".format(mid, stmt, ";" if (len(stmt) > 0 and stmt[-1] != ";") else "")
    res += "    default: {{{}{} break;}}\n".format(default_stmt, ";" if (len(default_stmt) > 0 and default_stmt[-1] != ";") else "")
    res += "}"
    return res

def insert_header(meta_mu_file, number_of_mutants):
    with open(meta_mu_file) as f:
        content = f.read()
    with open(meta_mu_file, "w") as f:
        f.write("unsigned long  klee_semu_GenMu_Mutant_ID_Selector = {};\n".format(number_of_mutants+1))
        f.write("void klee_semu_GenMu_Mutant_ID_Selector_Func (unsigned long fromID, unsigned long toID);\n")
        f.write("void klee_semu_GenMu_Post_Mutation_Point_Func (unsigned long fromID, unsigned long toID);\n")
        f.write(content)

def compute(meta_mu_out_file, original_src_file, mutants_src_dir, no_skip_non_function_mutants=False):

    mutant_src_file_list = []
    meta_mu_info_file = None
    for fn in os.listdir(mutants_src_dir):
        if fn == META_MU_INFO_FILE_NAME:
            meta_mu_info_file = os.path.join(mutants_src_dir, fn)
        else:
            mutant_src_file_list.append(os.path.join(mutants_src_dir, fn))
    assert meta_mu_info_file is not None, "no meta mu info file"
    assert len(mutant_src_file_list) > 0, "no mutant found"

    # Use diff to get (expr, expr-start, mut-expr) tuples
    mutant_list = MutantInfo.get_mutant_list(original_src_file, mutant_src_file_list, meta_mu_info_file, no_skip_non_function_mutants=no_skip_non_function_mutants)

    number_of_mutants = len(mutant_list)

    stmt_to_expr_to_mut = get_stmt_to_expr_to_mutant(mutant_list)
    
    with open(original_src_file) as f:
        orig_src_str = f.read()

    meta_mu_code = apply_metamu(stmt_to_expr_to_mut, orig_src_str)
    with open(meta_mu_out_file, "w") as f:
        f.write(meta_mu_code)

    insert_header(meta_mu_out_file, number_of_mutants)

    # Get and store mutants mapping
    mut_map_file = os.path.splitext(meta_mu_out_file)[0] + ".mutant_mapping.json"
    mutant_mapping = {}
    for mut in mutant_list:
        mutant_mapping[int(mut.int_id)] = mut.raw_id
    
    with open(mut_map_file, 'w') as f:
        json.dump(mutant_mapping, f, indent=2, sort_keys=True)

    print("\n=========================================================================")
    print("[INPUT] Original source file:                       ", original_src_file)
    print("[INPUT] MASS Generated mutants folder:              ", mutants_src_dir)
    print("")
    print("[OUTPUT] Meta mutant written in file:               ", meta_mu_out_file)
    print("[OUTPUT] mutant ID, mutant src map written in file: ", mut_map_file)
    print("=========================================================================\n")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("meta_mu_out_file", help="output meta mu file")
    parser.add_argument("original_src_file", help="original source file")
    parser.add_argument("mutants_src_dir", help="dircetory containing mutant files")
    parser.add_argument("--no_skip_non_function_mutants", action="store_true", help="disable skiping mutants not in functions (which are not supported by semu)")
    args = parser.parse_args()

    assert os.path.isdir(os.path.dirname(args.meta_mu_out_file)), "meta mu out file parent dir non existant"
    assert os.path.isfile(args.original_src_file), "original source file non existant"
    assert os.path.isdir(args.mutants_src_dir), "mutants src dir non existant"

    compute(args.meta_mu_out_file, args.original_src_file, args.mutants_src_dir, no_skip_non_function_mutants=args.no_skip_non_function_mutants)

if __name__ == "__main__":
    main()

