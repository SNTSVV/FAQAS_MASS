#! /usr/bin/env python

# Use libclang

# For example, for ASN case, runn as following
# >   ./generate_direct.py ../WORKSPACE/DOWNLOADED/casestudy/test.c direct " -I../WORKSPACE/DOWNLOADED/casestudy/" -c generate_template_config.json


import os
import re
import argparse
import subprocess
import json
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
{% for arg_name, initialization_code in initialized_input_arg_name_and_init_code %}
    {{ initialization_code }};
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


def repeat_format(fmt_str, single_val):
    return fmt_str.format(*([single_val]*len(fmt_str)))


RESULT_VAR_NAME = "result_faqas_semu"

TYPES_TO_INTCONVERT = "TYPES_TO_INTCONVERT"
TYPES_TO_PRINTCODE = "TYPES_TO_PRINTCODE"
OUT_ARGS_NAMES = "OUT_ARGS_NAMES"
IN_OUT_ARGS_NAMES = "IN_OUT_ARGS_NAMES"
TYPE_TO_INITIALIZATIONCODE = "TYPE_TO_INITIALIZATIONCODE"
TYPE_TO_SYMBOLIC_FIELDS_ACCESS = "TYPE_TO_SYMBOLIC_FIELDS_ACCESS"

# FIXME: Use the global config object in code
globalConfigObject = {
    # Specify a type as key and the type conversion template as value, 
    # Where the placeholder for the expression to convert should be specified as the string '{}'
    # e.g. "TYPES_TO_INTCONVERT": {"flag": "(int){}"},
    TYPES_TO_INTCONVERT: {},

    # Specify a type as key and the object printing code as value.
    # Where the placeholder for the object to print must be specified as the string '{}'
    # e.g. "TYPES_TO_PRINTCODE": {"struct XY *": "printf(\"FAQAS-SEMU-TEST_OUTPUT: X=%d, Y=%s\\n\", {}->x, {}->y)"}
    TYPES_TO_PRINTCODE: {},

    # Specify the names of function arguments that are used as function output (passed by reference for output only)
    # e.g. "OUT_ARGS_NAMES": ["pErrCode"],
    OUT_ARGS_NAMES: [],

    # Specify the names of function arguments that are used both as function input and output (passed by reference)
    # e.g. "IN_OUT_ARGS_NAMES": ["inoutArg"],
    IN_OUT_ARGS_NAMES: [],

    # Specify a type as key and the pre 'klee_make_symbolic' statement initialization code as value,
    # The placeholder for the object to initialize must be specified as the string '{}'
    # e.g. "TYPE_TO_INITIALIZATIONCODE": {"struct head": "{}.next = malloc(sizeof(struct head));\n{}.next->next = NULL;"}
    TYPE_TO_INITIALIZATIONCODE: {},

    # Specify how to make an object symbolic (specialy useful for objects that are initialized, like pointers).
    # The object type is the dict key and a dict of field accesses and their type is the dict value.
    # The placeholder for the object to make symbolic must be specified as the string '{}'
    # e.g. "TYPE_TO_SYMBOLIC_FIELDS_ACCESS": {"struct head": {"{}.data": "char [3]", "{}.next->data": "char [3]"}}
    TYPE_TO_SYMBOLIC_FIELDS_ACCESS: {}
}


def load_global_config(filename):
    global globalConfigObject
    with open(filename) as f:
        config_obj = json.load(f)
    
    expect_dict = (TYPES_TO_INTCONVERT, TYPES_TO_PRINTCODE, \
                    TYPE_TO_INITIALIZATIONCODE, TYPE_TO_SYMBOLIC_FIELDS_ACCESS)
    expect_list = (OUT_ARGS_NAMES, IN_OUT_ARGS_NAMES)
    for k,v in config_obj.items():
        if k in expect_dict:
            assert type(v) == dict, "A dict is expected as value for the keys {}".format(expect_dict)
        elif k in expect_list:
            assert type(v) == list, "A list is expected as value for the keys {}".format(expect_list)
        else:
            raise Exception("Invalid config key: {}".format(k))
    
    globalConfigObject.update(config_obj)

    assert len(set(globalConfigObject[OUT_ARGS_NAMES]) & set(globalConfigObject[IN_OUT_ARGS_NAMES])) == 0, \
                "Some arguments are both in OUT_ARGS_NAMES and IN_OUT_ARGS_NAMES"
    for k,v in globalConfigObject[TYPE_TO_SYMBOLIC_FIELDS_ACCESS].items():
        assert type(v) == dict, "Expecting a dict as values of dict for parameter {}".format(TYPE_TO_SYMBOLIC_FIELDS_ACCESS)


def is_primitive_type_get_fmt(type_name, obj_name=RESULT_VAR_NAME, obj_value=RESULT_VAR_NAME):
    printf_fmt = 'printf("FAQAS-SEMU-TEST_OUTPUT: %s = {}\\n", %s);' % (obj_name, obj_value)
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
    elif type_name == "enum" or type_name.startswith("enum "):
        return printf_fmt.format("%d")
    return None

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("source_file", metavar="source-file", help="source file containing the functions to test (all defined functions)")
    parser.add_argument("output_dir", metavar="output-dir", help="Directory to put the generated files")
    if USE_COMP_DB:
        parser.add_argument("compilation_db", metavar="compilation-db", help="Compilation database file")
    else:
        parser.add_argument("compilation_cflags", metavar="compilation-cflags", type=str, help="Compilation cflags (include dir, defines, ...)")
    parser.add_argument("-c", "--config-file", dest="config_file", help="Configuration file that speifies, in JSON format, the types conversion and printing formatting, as well as output in function arguments")
    args = parser.parse_args()

    if args.config_file:
        assert os.path.isfile(args.config_file), "The specified config file does not exist ({})".format(args.config_file)
        load_global_config(args.config_file)

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

    non_printing_templates = []
    for prototype in prototypes:
        code_filepath = os.path.join(args.output_dir, prototype.function_name + '.' + "wrapping_main.c")
        # checks
        ## check 1
        tmp = [None, None]
        for x_tmp_pos, (x_args_names, x_info) in enumerate([(globalConfigObject[OUT_ARGS_NAMES], ""), (globalConfigObject[IN_OUT_ARGS_NAMES], "input/")]):
            ooa_left = set(x_args_names) - set(prototype.get_argname_list())
            if len(ooa_left) > 0:
                tmp[x_tmp_pos] = {oa for oa in x_args_names if oa not in ooa_left}
                print("Warning: specified {}output args are not all used.".format(x_info),
                        "function name is {}.".format(prototype.function_name),
                        "function argument names are: {}.".format(prototype.get_argname_list()),
                        "These are not used: {}\n".format(ooa_left))
            else:
                tmp[x_tmp_pos] = set(x_args_names)
        used_out_only_args, used_inout_args = tmp
        used_outs_prints = []
        used_outs_args = used_out_only_args | used_inout_args
        for arg_name, type_name in prototype.get_argname_and_type_list():
            if arg_name in used_outs_args:
                # get print code
                prim_print_fmt = is_primitive_type_get_fmt(type_name, obj_name=arg_name, obj_value=arg_name)
                if type_name in globalConfigObject[TYPES_TO_PRINTCODE]:
                    used_outs_prints.append(repeat_format(globalConfigObject[TYPES_TO_PRINTCODE][prototype.return_type], arg_name))
                elif prim_print_fmt is not None:
                    used_outs_prints.append(prim_print_fmt)
                else:
                    raise Exception("The argument '{}' of type '{}' of function '{}' {}. {}".format(
                            arg_name,
                            type_name,
                            prototype.function_name,
                            "is not directly printable with printf",
                            "Add the print template of the type to the configuration."
                        )
                    )
        ## check2
        returns_void = False
        print_retval_stmts = []
        res_to_int = None
        if prototype.return_type == "void":
            returns_void = True
            print ("@void-returning-function: name={}, template={}".format(prototype.function_name, code_filepath))
        else:
            prim_print_fmt = is_primitive_type_get_fmt(prototype.return_type, obj_name=RESULT_VAR_NAME, obj_value=RESULT_VAR_NAME)
            if prototype.return_type in globalConfigObject[TYPES_TO_INTCONVERT]:
                res_to_int = globalConfigObject[TYPES_TO_INTCONVERT][prototype.return_type].format(RESULT_VAR_NAME)
            elif prim_print_fmt is not None:
                res_to_int = "(int){}".format(RESULT_VAR_NAME)
            else:
                raise Exception("The function return type for function '{}' is '{}' {}. {}".format(
                        prototype.function_name,
                        prototype.return_type,
                        "which is not directly convertible to int",
                        "Add the conversion template to the configuration."
                    )
                )
            if prototype.return_type in globalConfigObject[TYPES_TO_PRINTCODE]:
                print_retval_stmts.append(repeat_format(globalConfigObject[TYPES_TO_PRINTCODE][prototype.return_type], RESULT_VAR_NAME))
            elif prim_print_fmt is not None:
                print_retval_stmts.append(prim_print_fmt)
            else:
                raise Exception("The function return type for function '{}' is '{}' {}. {}".format(
                        prototype.function_name,
                        prototype.return_type,
                        "which is not directly printable with printf",
                        "Add the print template to the configuration."
                    )
                )

        arg_to_initcode  = []
        used_input_arg_name_and_type_list = []
        for arg_name, type_name in prototype.get_argname_and_type_list(discard=set(used_out_only_args)):
            if type_name in globalConfigObject[TYPE_TO_INITIALIZATIONCODE]:
                arg_to_initcode.append((arg_name, repeat_format(globalConfigObject[TYPE_TO_INITIALIZATIONCODE][type_name], arg_name)))
            if type_name in globalConfigObject[TYPE_TO_SYMBOLIC_FIELDS_ACCESS]:
                for field_access, field_type in globalConfigObject[TYPE_TO_SYMBOLIC_FIELDS_ACCESS][type_name].items():
                    used_input_arg_name_and_type_list.append((repeat_format(field_access, arg_name), field_type))
            else:
                used_input_arg_name_and_type_list.append((arg_name, type_name))

        all_prints = used_outs_prints + print_retval_stmts

        if len(all_prints) == 0 :
            non_printing_templates.append(prototype.function_name, code_filepath)

        code = Template(USED_TEMPLATE, trim_blocks=True, lstrip_blocks=True).render(
            function_return_type=prototype.return_type,
            arg_ptr_stripped_decl_list=prototype.get_arg_ptr_stripped_decl_list(),
            input_arg_name_and_type_list=used_input_arg_name_and_type_list,
            function_name=prototype.function_name,
            call_args_list=prototype.get_call_args_list(),
            output_out_args=all_prints,
            result_faqas_semu_to_int=res_to_int,
            returns_void=returns_void,
            source_file=args.source_file,
            initialized_input_arg_name_and_init_code=arg_to_initcode,
        )
        with open(code_filepath, 'w') as f:
            f.write(code)
    
    if len(non_printing_templates) > 0:
        print("Info: The following functions templates do not print:")
        for fname, ffile in non_printing_templates:
            print ("@non-printing-templates: functionName={}, template={}".format(fname, ffile))

    print("Done writing templates in folder {}".format(args.output_dir))

if __name__ == "__main__":
    main()
