#! /usr/bin/env python

# Use libclang

import os
import argparse

import clang.cindex

def get_prototype(func_decl):
    pass

def get_function_prototypes(source_file, compilation_db):
    index = clang.cindex.create()
    translation_unit = index.parse(source_file)
    func_definitions = []
    for elem in translation_unit.cursor.get_children():
        if elem.kind == clang.cindex.CursorKind.FUNCTION_DECL and elem.is_definition():
            func_definitions.append(list(elem.get_children())[0])
    function_protos = []
    for fp in func_definitions:
        function_protos.append(get_prototype(fp))
    

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("compilation_db", metavar="compilation-db", help="Compilation database file")
    parser.add_argument("source_file", metavar="source-file", help="source file containing the functions to test (all defined functions)")
    args = parser.parse_args()
    get_function_prototypes(args.source_file, args.compilation_db)

if __name__ == "__main__":
    main()
