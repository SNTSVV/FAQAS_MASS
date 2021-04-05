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
from itertools import groupby
from operator import itemgetter

#import difflib
import pandas as pd

META_MU_INFO_FILE_NAME = "meta-mu.info"

StmtInfo = collections.namedtuple("StmtInfo", ["start_index", "end_index"])
ChangedInfo = collections.namedtuple("ChangedInfo", ["start_index", "end_index", "filename", "mut_str"]) #End index character is excluded

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
    def get_mutant_list (cls, original_src_file, mutant_src_file_list, full_sdl_mutant_src_file_list, meta_mu_info_file, sdl_meta_mu_info_file):
        stmt_list = cls._get_stmt_list(original_src_file, full_sdl_mutant_src_file_list, sdl_meta_mu_info_file)
        mut_list = []
        changed_list = cls._get_changed_list (original_src_file, mutant_src_file_list, meta_mu_info_file,deletion_only=False)
        changed_list.sort(key=lambda x: (x.start_index, x.end_index))
        stmt_i = 0
        for int_idx, ci in enumerate(changed_list):
            stmt_info = None
            while stmt_i < len(stmt_list):
                if ci.start_index < stmt_list[stmt_i].start_index:
                    assert False, "mutant not in any stmt ({})".format(ci.filename)
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
            assert stmt_info is not None, "Could not find stmt for mutant {}".format(ci.filename)

            mut_list.append(
                MutantInfo(
                    stmt_info=stmt_info,
                    changed_info=ci,
                    int_id=int_idx+1
                )
            )
        

        return mut_list

    @staticmethod
    def _get_stmt_list (original_src_file, full_sdl_mutant_src_file_list, meta_mu_info_file):
        """
        take original and SDL mutants, anf find stmts
        """
        stmt_list = []
        for ci in MutantInfo._get_changed_list (original_src_file, full_sdl_mutant_src_file_list, meta_mu_info_file, deletion_only=True):
            stmt_list.append(
                StmtInfo(
                    start_index=ci.start_index,
                    end_index=ci.end_index
                )
            )
        stmt_list.sort(key=lambda x: (x.start_index, x.end_index))

        return stmt_list

    @staticmethod
    def _get_changed_list (original_src_file, mutant_src_file_list, meta_mu_info_file, deletion_only=False):
        # ensure no basename duplicate
        assert len(set([os.path.basename(fn) for fn in mutant_src_file_list])) == len(mutant_src_file_list), "duplicate mutant basename"

        #seq_matcher = SequenceMatcher()
        with open(original_src_file) as f:
            orig_str = f.read()
            f.seek(0)
            orig_lines_idx = [None, 0]
            for l in f.readlines():
                orig_lines_idx.append(orig_lines_idx[-1] + len(l))
            del orig_lines_idx[-1]
            #print('DBG', len(orig_lines_idx)-1, orig_lines_idx[-1], len(l), len(orig_str), l)

        meta_mu_info_df = pd.read_csv(meta_mu_info_file)
        assert set(meta_mu_info_df) == {"MUTANT_FILE", "START_LINE", "START_COL", "SIZE"}, "Invalid meta mu info"
        mut_info_map = {}
        for row in meta_mu_info_df.itertuples():
            row = row._asdict()
            fn = os.path.basename(row["MUTANT_FILE"])
            assert fn not in mut_info_map, "duplicate mutant basename in meta mu info ()".format(meta_mu_info_file)
            mut_info_map[fn] = {k: row[k] for k in ("START_LINE", "START_COL", "SIZE")}

        #seq_matcher.set_seq1(orig_str)
        changed_list = []
        for mutant_src_file in mutant_src_file_list:
            mutant_basename = os.path.basename(mutant_src_file)
            mut_info = mut_info_map[mutant_basename]
            try:
                mut_idx = orig_lines_idx[mut_info["START_LINE"]] + mut_info["START_COL"] - 1
            except IndexError as e:
                print("\n# mut_info['START_LINE'] is", mut_info["START_LINE"], ", len(orig_lines_idx) is", len(orig_lines_idx), "\n")
                raise
            mut_after_end = mut_idx + mut_info["SIZE"]

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
                                    and (r_dat[1] - o_dat[1]) * (r_dat[0] - o_dat[0]) >= 0:
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
                            assert False, "Got crosing, strange. (TODO: expand the mutant replacement code and uncomment this)"
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
        code[i+1] = {(i+1, i+2): orig_src_str[i]}
    #print(len(orig_src_str) + 1)

    def get_code(start, after_end):
        # get the larget interval starting at start
        largest = max(code[start].keys(), key=lambda x:(int(after_end>=x[1]), x[1]-after_end))
        if largest[1] < after_end:
            return code[start][largest] + get_code(largest[1], after_end)
        return code[start][largest]

    def set_code(start, after_end, code_str):
        code[start][(start, after_end)] = code_str
            
    stmt2mutant_ids = {}
    for stmt in stmts:
        stmt2mutant_ids[stmt] = []
        exprs = sorted(list(stmt_to_expr_to_mut[stmt]), reverse=True, key=lambda x:(x[0], -x[1]))
        for expr in exprs:
            existing_code = get_code(expr[0], expr[1])
            id2repl = {mut.int_id: mut.changed_info.mut_str for mut in stmt_to_expr_to_mut[stmt][expr]}
            if stmt_to_expr_to_mut[stmt][expr][0]._is_whole_stmt():
                mutated = compute_switch_stmt(id2repl, existing_code)
            else:
                mutated = compute_selection_expr(existing_code, id2repl)
            stmt2mutant_ids[stmt] += list(id2repl)
    for stmt, mut_list in stmt2mutant_ids.items():
        existing_code = get_code(stmt.start_index, stmt.end_index)
        sel_stmts = compute_mutation_points(mut_list)
        set_code(stmt.start_index, stmt.end_index, sel_stmts + existing_code)
    return get_code(1, len(orig_src_str)+1)

MID_SEL_GLOBAL = "klee_semu_GenMu_Mutant_ID_Selector"

def compute_mutation_points(mut_list):
    s_list = sorted(mut_list)
    ranges =[]

    for k,g in groupby(enumerate(data),lambda x:x[0]-x[1]):
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
    res = "(" + orig_str + ")"
    for mid, mexpr in sorted(mid2expr.items()):
        res = "({}=={}?({}):{})".format(MID_SEL_GLOBAL, mid, mexpr, res)

def compute_switch_stmt(id2stmt, default_stmt):
    res = "switch (klee_semu_GenMu_Mutant_ID_Selector) {\n"
    for mid, stmt in id2stmt.items():
        res += "    case {}: {}{} break".format(mid, stmt, ";" if stmt[-1] != ";" else "")
    res += "    default: {}{} break".format(default_stmt, ";" if default_stmt[-1] != ";" else "")
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

def compute(meta_mu_out_file, original_src_file, mutants_src_dir, full_sdl_src_dir):

    full_sdl_mutant_src_file_list = []
    sdl_meta_mu_info_file = None
    for fn in os.listdir(full_sdl_src_dir):
        if fn == META_MU_INFO_FILE_NAME:
            sdl_meta_mu_info_file = os.path.join(full_sdl_src_dir, fn)
        else:
            full_sdl_mutant_src_file_list.append(os.path.join(full_sdl_src_dir, fn))
    assert sdl_meta_mu_info_file is not None, "no meta mu info file for full sdl"
    assert len(full_sdl_mutant_src_file_list) > 0, "no full sdl mutant found"

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
    mutant_list = MutantInfo.get_mutant_list(original_src_file, mutant_src_file_list, full_sdl_mutant_src_file_list, meta_mu_info_file, sdl_meta_mu_info_file)

    number_of_mutants = len(mutant_list)

    stmt_to_expr_to_mut = get_stmt_to_expr_to_mutant(mutant_list)
    
    with open(original_src_file) as f:
        orig_src_str = f.read()

    meta_mu_code = apply_metamu(stmt_to_expr_to_mut, orig_src_str)
    with open(meta_mu_out_file, "w") as f:
        f.write(meta_mu_code)

    insert_header(meta_mu_out_file, number_of_mutants)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("meta_mu_out_file", help="output meta mu file")
    parser.add_argument("original_src_file", help="original source file")
    parser.add_argument("mutants_src_dir", help="dircetory containing mutant files")
    parser.add_argument("full_sdl_src_dir", help="directory containing full sdl mutants")
    args = parser.parse_args()

    assert os.path.isdir(os.path.dirname(args.meta_mu_out_file)), "meta mu out file parent dir non existant"
    assert os.path.isfile(args.original_src_file), "original source file non existant"
    assert os.path.isdir(args.mutants_src_dir), "mutants src dir non existant"
    assert os.path.isdir(args.full_sdl_src_dir), "full_sdl_src_dir non existant"

    compute(args.meta_mu_out_file, args.original_src_file, args.mutants_src_dir, args.full_sdl_src_dir)

if __name__ == "__main__":
    main()

