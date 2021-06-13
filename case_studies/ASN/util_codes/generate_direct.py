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
{% endfor %}

    result_faqas_semu = {{ function_name }}}({{ call_args_list|join(", ") }});
{%for ooa in output_out_args %}
    {{ ooa }}
{% endfor %}
    return {{ result_faqas_semu_to_int }};
}

"""

class Prototype:
    def __init__(self, return_type, function_name, params_name_and_decl):
        self.return_type = return_type
        self.function_name = function_name
        # list of tuples of: def type
        self.params_name_and_decl = params_name_and_decl

    def get_call_args_list(self):
        return [tup[4] for tup in self.params_name_and_decl]

    def get_arg_ptr_stripped_decl_list(self):
        return [tup[3] for tup in self.params_name_and_decl]

    def get_argname_list(self, discard=[]):
        return [tup[0] for tup in self.params_name_and_decl if tup[0] not in discard]

def strip_type_qualifier(in_type_str):
    split = in_type_str.split()
    while len(split) > 0:
        if split[0] in ('const', 'volatile', 'restrict'):
            split.pop(0)
        else:
            break
    return " ".join(split)

def strip_one_ptr(in_type):
    tmp = in_type.get_pointee()
    if tmp.spelling:
        return True, tmp
    return False, in_type

def get_decl(name, type_str):
    ind = type_str.find('[')
    if ind >= 0:
        return type_str[:ind] + name + type_str[ind:]
    return type_str + " " + name

def get_prototype(func_decl):
    func_name = func_decl.spelling
    ret_type = strip_type_qualifier(func_decl.result_type.get_canonical().spelling)
    params_name_decl = []
    for child in func_decl.get_arguments():
        can_type = child.type.get_canonical()
        c_type_str = strip_type_qualifier(can_type.spelling)
        has_ptr, ptr_stripped = strip_one_ptr(can_type)
        un_ptr_type = strip_type_qualifier(ptr_stripped.spelling)
        un_ptr_decl = get_decl(child.spelling, un_ptr_type)
        call_arg = "&" + child.spelling if ptr_stripped else child.spelling
        params_name_decl.append((child.spelling, c_type_str, un_ptr_type, un_ptr_decl, call_arg))
    return Prototype(ret_type, func_name, params_name_decl)

def get_function_prototypes(source_file, compilation_db):
    index = clang.cindex.Index.create()
    translation_unit = index.parse(source_file)
    func_definitions = []
    for elem in translation_unit.cursor.get_children():
        if elem.kind == clang.cindex.CursorKind.FUNCTION_DECL and elem.is_definition():
            func_definitions.append(elem)
            if elem.type.is_function_variadic():
                print("WARNING: The function {} is variadic. manually check for the call statement to pass more arguments".format(elem.spelling))
    function_protos = []
    for cursor in func_definitions:
        function_protos.append(get_prototype(cursor))
    return function_protos

OUT_ARGS = {"errCode": 'printf("%d\n", errCode);'}
RESULT_TYPE = "flag"
RESULT_TO_INT = "(int)result_faqas_semu"

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
        # checks
        ## check 1
        ooa_left = set(OUT_ARGS.keys()) - set(prototype.get_argname_list())
        if len(ooa_left) > 0:
            used_out_args = {k:v for k,v inO OUT_ARGS.items() if k not in ooa_left}
            print("warning: specified ooa are not all used. There are not used: ".format(ooa_left))
        else:
            used_out_args = dict(OUT_ARGS)
        ## check2
        if prototype.return_type != RESULT_TYPE:
            res_to_int = input("The function return type for function '{}' is not '{}' but is '{}'. {}".format(
                    prototype.function_name,
                    RESULT_TYPE,
                    prototype.return_type,
                    "Input the conversion to int for variable 'result_faqas_semu': "
                )
            )
        else:
            res_to_int = RESULT_TO_INT

        code = Template(USED_TEMPLATE).render(
            function_return_type=prototype.return_type,
            arg_ptr_stripped_decl_list=prototype.get_arg_ptr_stripped_decl_list(),
            input_arg_name_list=prototype.get_argname_list(discard=set(used_out_args.keys())),
            function_name=prototype.function_name,
            call_args_list=prototype.get_call_args_list(),
            output_out_arg=list(used_out_args.values()),
            result_faqas_semu_to_int=res_to_int
        )
        with open(code_filepath) as f:
            f.write(code)
    
    print("Done writing templates in folder {}".format(args.output_dir))

if __name__ == "__main__":
    main()
