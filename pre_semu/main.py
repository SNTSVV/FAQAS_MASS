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

import argparse
import collections
import difflib

StmtInfo = collections.namedtuple("StmtInfo", ["start_index", "length"])

def get_stmt_list (original_src_file, mutant_src_file_list):
    """
    take original and SDL mutants, anf find stmts
    """
    seq_matcher = SequenceMatcher()
    with open(original_src_file) as f:
        orig_str = f.read()
    seq_matcher.set_seq1(orig_str)
    stmt_list = []
    for mutant_src_file in mutant_src_file_list:
        with open(mutant_src_file) as f:
            mutant_str = f.read()
        # compute the deleted statement location
        seq_matcher.set_seq2(mutant_str)
        match_blocks = seq_matcher.get_matching_blocks()
        orig_idx = mut_idx = 0
        for mb in match_blocks:
            # a is orig, b is mut
            assert mb.b == mut_idx, "unexpected gap in mutant-orig at index".format(mut_idx)
            if orig_idx < mb.a and mb.size > 0:
                stmt_list.append(
                    StmtInfo(
                        start_index=orig_idx,
                        length=(mb.a - orig_idx)
                    )
                )
            orig_idx = mb.a + mb.size
            mut_idx = mb.b + mb.size
    return stmt_list
        

def main():
    # Call SDL generation

    # use diff to get stmt start, stmt len pairs

    # Call MASS generation

    # Use diff to get (expr, expr-start, mut-expr) tuples

    # cluster muts expr and stmt

    # apply for meta-mu using above info

if __name__ == "__main__":
    main()

