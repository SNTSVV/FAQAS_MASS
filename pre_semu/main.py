#! /user/bin/env python

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
import difflib

StmtInfo = collections.namedtuple("StmtInfo", ["start_index", "end_index"])
ChangedInfo = collections.namedtuple("ChangedInfo", ["start_index", "end_index", "filename", "mut_str"]) #End index character is excluded

class MutantInfo:
    def __init__(self, stmt_info, changed_info, int_id):
        self.raw_id = self._computer_raw_id(changed_info.filename)
        self.int_id = int_id
        self.stmt_info = stmt_info
        self.changed_info = changed_info

    def _computer_raw_id(self, mut_filename):
        return os.path.basename(mut_filename)

    @classmethod
    def get_mutant_list (cls, original_src_file, mutant_src_file_list, full_sdl_mutant_src_file_list):
        stmt_list = cls.get_stmt_list(original_src_file, full_sdl_mutant_src_file_list)
        mut_list = []
        chaged_list = _get_changed_list (original_src_file, mutant_src_file_list, deletion_only=False)
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
    def _get_stmt_list (original_src_file, full_sdl_mutant_src_file_list):
        """
        take original and SDL mutants, anf find stmts
        """
        stmt_list = []
        for ci in _get_changed_list (original_src_file, full_sdl_mutant_src_file_list, deletion_only=True):
            stmt_list.append(
                StmtInfo(
                    start_index=ci.start_index,
                    end_index=ci.end_index
                )
            )
        stmt_list.sort(key=lambda x: (x.start_index, x.end_index))

        return stmt_list

    @staticmethod
    def _get_changed_list (original_src_file, mutant_src_file_list, deletion_only=False):
        seq_matcher = SequenceMatcher()
        with open(original_src_file) as f:
            orig_str = f.read()
        seq_matcher.set_seq1(orig_str)
        changed_list = []
        for mutant_src_file in mutant_src_file_list:
            with open(mutant_src_file) as f:
                mutant_str = f.read()
            # compute the deleted statement location
            seq_matcher.set_seq2(mutant_str)
            match_blocks = seq_matcher.get_matching_blocks()
            orig_idx = mut_idx = 0
            for mb in match_blocks:
                # a is orig, b is mut
                if deletion_only:
                    assert mb.b == mut_idx, "unexpected gap in mutant-orig at index".format(mut_idx)
                if orig_idx < mb.a and mb.size > 0:
                    changed_list.append(
                        ChangedInfo(
                            start_index=orig_idx,
                            end_index=mb.a, 
                            filename=mutant_src_file, 
                            mut_str=mutant_str[mut_idx: mb.b - mut_idx]
                        )
                    )
                orig_idx = mb.a + mb.size
                mut_idx = mb.b + mb.size
        return changed_list
        
def get_stmt_to_expr_to_mutant(mutant_list):
    """
    Take the mutant list, group by expression and statement, alterating mutant info accordingly
    """
    stmt_to_expr_to_mut = {}
    
    stmt_to_muts = {}
    for mut_info in mutant_list:
        if mut_info.stmt_info not in stmt_to_muts:
            stmt_to_muts[mut_info.stmt_info] = []]
            stmt_to_expr_to_mut[mut_info.stmt_info] = {}
        stmt_to_muts[mut_info.stmt_info].append(mut_info)
        
        # get expressions
        for stmt, mut_list in stmt_to_muts.items():
            # TODO
            
            expr_to_mut = 
    # TODO
    return stmt_to_expr_to_mut

def apply_metamu(stmt_to_expr_to_mut, orig_src_str):
    """
    take stmt to expr to mutant and the original source code string and return the meta-mutant source code
    """
    pass #TODO

MID_SEL_GLOBAL = "klee_semu_GenMu_Mutant_ID_Selector"

def compute_mutation_point_str(mut_start_id, mut_end_id):
    return "klee_semu_GenMu_Mutant_ID_Selector_Func({},{});".format(mut_start_id, mut_end_id)
    #TODO (Maybe the mutants with not corresponding SDL stmt are those on stmts that cannot be deleted)

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

def main():
    # Call SDL generation

    # use diff to get stmt start, stmt len pairs

    # Call MASS generation

    # Use diff to get (expr, expr-start, mut-expr) tuples

    # cluster muts expr and stmt

    # apply for meta-mu using above info

if __name__ == "__main__":
    main()

