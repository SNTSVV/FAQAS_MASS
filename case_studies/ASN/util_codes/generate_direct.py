#! /usr/bin/env python

# Use libclang

import os
import argparse
from jinja2 import Template

import clang.cindex

USED_TEMPLATE = """
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c*/

#include "asn1crt.c"
#include "asn1crt_encoding.c"
#include "asn1crt_encoding_uper.c"

#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    {{ function_return_type }} result_faqas_semu;
{% for arg_ptr_stripped_decl in arg_ptr_stripped_decl_list %}
    {{ arg_ptr_stripped_decl }};
{% endfor %}
{% for arg_name in input_arg_name_list %}
    klee_make_symbolic(&{{ arg_name }}, sizeof({{ arg_name }}), "{{ arg_name }}");
{% endfor %}}

    result_faqas_semu = {{ function_name }}}({{ call_args_list|join(", ") }});
    {{ output_out_arg }}
    return {{ result_faqas_semu_to_int }};
}

"""

class Prototype:
    return_type = None
    function_name = None
    # list of tuples of: def type
    params_type_name = []

    @staticmethod
    def type_is_ptr(arg_decl):
        return '*' in arg_decl

    @staticmethod
    def get_ptr_stripped(arg_decl):
        last_star_index = arg_decl.rfind("*")
        if last_star_index < 0:
            return arg_decl
        return arg_decl[:last_star_index] + ' ' + arg_decl[last_star_index + 1:]

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
    parser.add_argument("output_dir", metavar="output-dir", help="Directory to put the generated files")
    args = parser.parse_args()
    if not os.path.join(args.compilation_db):
        assert False, "The specified compilation_db file does not exist"
    if not os.path.join(args.source_file):
        assert False, "The specified source file does not exist" 
    prototypes = get_function_prototypes(args.source_file, args.compilation_db)

    if not os.path.join(args.output_dir):
        os.mkdir(args.output_dir)

    for prototype in prototypes:
        code_filepath = os.path.join(args.output_dir, prototype.function_name + '.' + "wrapping_main.c")
        code = Template(USED_TEMPLATE).render(
            something="World"
        )
        with open(code_filepath) as f:
            f.write(code)
    
    print("Done writing templates in folder {}".format(args.output_dir))

if __name__ == "__main__":
    main()
