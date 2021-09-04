#! /usr/bin/env python

# Use libclang

# For example, for ASN case, runn as following
# >   ./generate_direct.py ../WORKSPACE/DOWNLOADED/casestudy/test.c direct " -I../WORKSPACE/DOWNLOADED/casestudy/"


import os
import re
import argparse
import subprocess
from jinja2 import Template

import clang.cindex

USED_TEMPLATE = """
/* Wrapping main template for the function {{ function_name }} defined in the file {{ source_file }} */
/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */

#include <stdio.h>
#include <string.h>

#include "asn1crt.c"
#include "asn1crt_encoding.c"
#include "asn1crt_encoding_uper.c"

#include "klee/klee.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
{% if not returns_void %}

    // Declare variable to hold function returned value
    {{ function_return_type }} result_faqas_semu;
{% endif %}

    // Declare arguments and make input ones symbolic
{% for arg_ptr_stripped_decl in arg_ptr_stripped_decl_list %}
    {{ arg_ptr_stripped_decl }};
{% endfor %}
{% for arg_name, type_name in input_arg_name_and_type_list %}
    memset(&{{ arg_name }}, 0, sizeof({{ arg_name }}));
{% endfor %}
{% for arg_name, type_name in input_arg_name_and_type_list %}
    klee_make_symbolic(&{{ arg_name }}, sizeof({{ arg_name }}), "{{ arg_name }}"); //{{ type_name }}
{% endfor %}

    // Call function under test
{% if not returns_void %}
    result_faqas_semu = {{ function_name }}({{ call_args_list|join(", ") }});
{% else %}
    {{ function_name }}({{ call_args_list|join(", ") }});
{% endif %}

    // Make some output
{%for ooa in output_out_args %}
    {{ ooa }}
{% endfor %}
{% if not returns_void %}
    return {{ result_faqas_semu_to_int }};
{% else %}
    return 0;
{% endif %}
}

"""

USE_COMP_DB = False

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

    def get_argname_and_type_list(self, discard=[]):
        return [(tup[0], tup[2]) for tup in self.params_name_and_decl if tup[0] not in discard]

def strip_type_qualifier(in_type_str):
    split = in_type_str.split()
    while len(split) > 0:
        if split[0] in ('const', 'volatile', 'restrict'):
            split.pop(0)
        else:
            break
    tmp_str = " ".join(split)

    split = re.findall(r"\w+|[^\w\s]+", tmp_str)
    tmp_str = " ".join(list(filter(lambda a: a != 'const', split)))
    
    return tmp_str

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
        call_arg = "&" + child.spelling if has_ptr else child.spelling
        params_name_decl.append((child.spelling, c_type_str, un_ptr_type, un_ptr_decl, call_arg))
    return Prototype(ret_type, func_name, params_name_decl)

def get_standard_includes():
    """ libclang has problem locating some standard include. Help it
    """
    cmd = ["cc", "-E", "-x", "c", "-v", "/dev/null"]
    log = subprocess.check_output(
        cmd,
        stderr=subprocess.STDOUT
    )
    log = log.decode('UTF-8', 'backslashreplace')
    started = False
    include_dirs = []
    for line in log.splitlines():
        if line.strip() == "#include <...> search starts here:":
            started = True
            continue
        if line.strip() == "End of search list.":
            started = False
            continue
        if started:
            include_dirs.append(line.strip())
            
    return include_dirs

def get_function_prototypes(source_file, compilation_info):
    index = clang.cindex.Index.create()
    if USE_COMP_DB:
        assert False, "Using compilation DB not yet implemented. FIXME"
    else:
        translation_unit = index.parse(source_file, args=[v for v in compilation_info.split() if v.strip()] + ["-I"+v for v in get_standard_includes()])
    func_definitions = []
    for elem in translation_unit.cursor.get_children():
        if elem.kind == clang.cindex.CursorKind.FUNCTION_DECL and elem.is_definition():
            if elem.location.file.name != source_file:
                # The function must be in this source file
                continue
            func_definitions.append(elem)
            if elem.type.is_function_variadic():
                print("WARNING: The function {} is variadic. manually check for the call statement to pass more arguments".format(elem.spelling))
    function_protos = []
    for cursor in func_definitions:
        function_protos.append(get_prototype(cursor))
    return function_protos

OUT_ARGS = {"pErrCode": 'printf("FAQAS-SEMU-TEST_OUTPUT: pErrCode = %d\\n", pErrCode);'}
RESULT_TYPE = "flag"
RESULT_TO_INT = "(int)result_faqas_semu"
RESULT_OUT = 'printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = %d\\n", result_faqas_semu);'

def is_primitive_type_get_fmt(type_name):
    printf_fmt = 'printf("FAQAS-SEMU-TEST_OUTPUT: result_faqas_semu = {}\\n", result_faqas_semu);'
    type_list = {
        "_Bool": "%d",

        "char": "%d", 
        "unsigned char": "%d", 
        "signed char": "%d", 

        "int": "%d", 
        "signed": "%d", 
        "signed int": "%d", 
        "unsigned": "%u", 
        "unsigned int": "%u", 

        "short": "%hi", 
        "signed short": "%hi", 
        "unsigned short": "%hu", 
        "short int": "%hi", 
        "signed short int": "%hi", 
        "unsigned short int": "%hu", 

        "long": "%ld", 
        "signed long": "%ld", 
        "unsigned long": "%lu", 
        "long int": "%ld", 
        "signed long int": "%ld", 
        "unsigned long int": "%lu", 
        
        "long long": "%lld", 
        "signed long long": "%lld", 
        "unsigned long long": "%llu", 
        "long long int": "%lld", 
        "signed long long int": "%lld", 
        "unsigned long long int": "%llu", 
        
        "float": "%g", 
        "double": "%G", 
        "long double": "%LG"
    }
    type_name = " ".join(type_name.split())
    if type_name in type_list:
        return printf_fmt.format(type_list[type_name])
    return None

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source_file", metavar="source-file", help="source file containing the functions to test (all defined functions)")
    parser.add_argument("output_dir", metavar="output-dir", help="Directory to put the generated files")
    if USE_COMP_DB:
        parser.add_argument("compilation_db", metavar="compilation-db", help="Compilation database file")
    else:
        parser.add_argument("compilation_cflags", metavar="compilation-cflags", type=str, help="Compilation cflags (include dir, defines, ...)")
    args = parser.parse_args()
    
    if not os.path.isfile(args.source_file):
        assert False, "The specified source file does not exist"
    if USE_COMP_DB:
        if not os.path.isfile(args.compilation_db):
            assert False, "The specified compilation_db file does not exist" 
        prototypes = get_function_prototypes(args.source_file, args.compilation_db)
    else:
        prototypes = get_function_prototypes(args.source_file, args.compilation_cflags)

    if not os.path.isdir(args.output_dir):
        os.mkdir(args.output_dir)

    for prototype in prototypes:
        code_filepath = os.path.join(args.output_dir, prototype.function_name + '.' + "wrapping_main.c")
        # checks
        ## check 1
        ooa_left = set(OUT_ARGS.keys()) - set(prototype.get_argname_list())
        if len(ooa_left) > 0:
            used_out_args = {k:v for k,v in OUT_ARGS.items() if k not in ooa_left}
            print("warning: specified output args are not all used.",
                    "function name is {}.".format(prototype.function_name),
                    "function argument names are: {}.".format(prototype.get_argname_list()),
                    "These are not used: {}\n".format(ooa_left))
        else:
            used_out_args = dict(OUT_ARGS)
        ## check2
        returns_void = False
        print_retval_stmts = []
        if prototype.return_type == "void":
            returns_void = True
        elif prototype.return_type != RESULT_TYPE and is_primitive_type_get_fmt(prototype.return_type) is None:
            res_to_int = input("The function return type for function '{}' is not '{}' but is '{}'. {}".format(
                    prototype.function_name,
                    RESULT_TYPE,
                    prototype.return_type,
                    "Input the conversion to int for variable 'result_faqas_semu': "
                )
            )
        else:
            res_to_int = RESULT_TO_INT
            if prototype.return_type == RESULT_TYPE:
                print_retval_stmts.append(RESULT_OUT)
            else:
                print_retval_stmts.append(is_primitive_type_get_fmt(prototype.return_type))

        code = Template(USED_TEMPLATE, trim_blocks=True, lstrip_blocks=True).render(
            function_return_type=prototype.return_type,
            arg_ptr_stripped_decl_list=prototype.get_arg_ptr_stripped_decl_list(),
            input_arg_name_and_type_list=prototype.get_argname_and_type_list(discard=set(used_out_args.keys())),
            function_name=prototype.function_name,
            call_args_list=prototype.get_call_args_list(),
            output_out_args=list(used_out_args.values()) + print_retval_stmts,
            result_faqas_semu_to_int=res_to_int,
            returns_void=returns_void,
            source_file=args.source_file
        )
        with open(code_filepath, 'w') as f:
            f.write(code)
    
    print("Done writing templates in folder {}".format(args.output_dir))

if __name__ == "__main__":
    main()
