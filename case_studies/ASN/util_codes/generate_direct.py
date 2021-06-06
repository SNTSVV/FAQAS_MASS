#! /usr/bin/env python

# Use libclang

import os
import argparse

import clang.cindex

def get_function_prototypes(source_file, compilation_db):
    index = clang.cindex.create()
    translation_unit = index.parse(source_file)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("compilation_db", metavar="compilation-db", help="Compilation database file")
    parser.add_argument("source_file", metavar="source-file", help="source file containing the functions to test (all defined functions)")
    args = parser.parse_args()
    get_function_prototypes(args.source_file, args.compilation_db)

if __name__ == "__main__":
    main()
