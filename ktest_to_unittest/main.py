#! /usr/bin/env python

import os
import binascii
import struct
import argparse

import pandas as pd

from muteria.drivers.testgeneration.testcase_formats.ktest.utils import ConvertCollectKtestsSeeds


def get_type_val(type_name, int_val, uint_val, decimal_val):
    if int_val is not None and type_name in ("_Bool",):
        return "Bool val is {}".format(int_val)
    if int_val is not None and type_name in ("char", "unsigned char", "signed char"):
        return "Char ASCII val is {}".format(int_val)
    if int_val is not None and (type_name == "enum" or type_name.startswith("enum "):
        return "Enum int val is {}".format(int_val)
    if int_val is not None and type_name in ("int", "signed", "signed int", 
                                    "short", "signed short", "short int", "signed short int", 
                                    "long", "signed long", "long int", "signed long int", 
                                    "long long", "signed long long", "long long int", "signed long long int"):
        return "Integer val is {}".format(int_val)
    if uint_val is not None and type_name in ("unsigned", "unsigned int", "unsigned short", "unsigned short int", 
                                    "unsigned long", "unsigned long int", "unsigned long long", "unsigned long long int"):
        return "Unsigned val is {}".format(uint_val)
    if decimal_val is not None and type_name in ("float", "double", "long double"):
        return "Decimal val is {}".format(decimal_val)
    return None


def extract_input_from_ktest(path_to_ktest_file):
    path_to, ktest_file = os.path.split(path_to_ktest_file)

    c_obj = ConvertCollectKtestsSeeds()
    _, dat = c_obj._loadAndGetSymArgsFromKleeKTests([ktest_file], path_to, must_have_model_version=False)

    hex_array_res = {}
    hex_res = {}
    int_res = {}
    uint_res = {}
    decimal_res = {}
    for name, val in dat['KTEST-OBJ'][0].objects:
        name = name.decode('UTF-8', 'backslashreplace')
        hexval = binascii.hexlify(val).decode('ascii')
        intval = None
        uintval = None
        for n, m in [(1, 'b'), (2, 'h'), (4, 'i'), (8, 'q')]:
            if len(val) == n:
                intval = struct.unpack(m, val)[0]
                uintval = struct.unpack(m.upper(), val)[0]
                break
        hex_res[name] = hexval
        int_res[name] = intval
        uint_res[name] = uintval
        if len(val) == 4:
            decimal_res[name] = struct.unpack('f', val)[0]
        elif len(val) == 8:
            decimal_res[name] = struct.unpack('d', val)[0]
        else:
            decimal_res[name] = None
    
        assert len(hex_res[name]) % 2 == 0, "Error, invalid hex_res (non even len)"
        hex_array_res[name] = ["0x"+hex_res[name][i:i+2] for i in range(0, len(hex_res[name]), 2)]

    return hex_array_res, hex_res, int_res, uint_res, decimal_res


def apply_input_to_template(result_file_path, template_file_path, path_to_ktest_file):
    hex_array_res, hex_res, int_res, uint_res, decimal_res = extract_input_from_ktest(path_to_ktest_file)

    with open(template_file_path) as f:
        template_str = f.read()

    result_lines_list = template_str.splitlines()
    
    #Create result from template, using the data
    ## 1. Remove <klee.h>
    klee_h_line = None
    for pos, line in enumerate(result_lines_list):
        line = line.strip()
        if line.startswith('#include "klee/klee.h"'):
            klee_h_line = pos
            break
    assert klee_h_line is not None, "no klee.h include"
    del result_lines_list[klee_h_line]

    ## 2. locate input vars in klee_make_symbolic
    klee_make_sym_locs = []
    for pos, line in enumerate(result_lines_list):
        line = line.strip()
        if line.startswith("klee_make_symbolic("):
            klee_make_sym_locs.append(pos)

    ## data setting decl
    data_decl = []
    missing_inputs_pos = []
    for kms in klee_make_sym_locs:
        tmp = result_lines_list[kms].split(',')
        name = tmp[2].split('"')[1]
        if name not in hex_array_res:
            missing_inputs_pos.append(kms)
            continue
        dat_name = name + "_faqas_semu_test_data"
        data_decl.append("    const unsigned char " + dat_name + "[] = {" + ", ".join(hex_array_res[name]) + "};")
        ### replace klee_make_symbolics with memcpy
        mcpy = tmp[0].replace("klee_make_symbolic(", "memcpy(") + ", " + dat_name + ',' + tmp[1] + ");"
        result_lines_list[kms] = mcpy
        if "//" in tmp[-1]:
            type_name = tmp[-1].split('//')[-1].strip()
            type_val = get_type_val(type_name, int_res[name], uint_res[name], decimal_res[name])
            if type_val is not None:
                result_lines_list[kms] += " // {}".format(type_val)

    ## 3. Delete missing input klee_make_symbolic and add hex array for existing inputs
    missing_inputs_pos.sort(reverse=True)
    klee_make_sym_locs.sort()
    for pos in missing_inputs_pos:
        del result_lines_list[pos]
    result_lines_list[klee_make_sym_locs[0]: klee_make_sym_locs[0]] = data_decl

    # Write result
    with open(result_file_path, "w") as f:
        f.write("\n".join(result_lines_list))


def produce_unittests_from_ktest_dir(outdir, indir, template_file_path):
    ktest_list = []
    mut_test_map_file_list = []

    # get the list of ktest files and ktest to mutant list mapping files
    for fname in os.listdir(indir):
        if fname.endswith(".ktest"):
            ktest_list.append(os.path.join(indir, fname))
        elif fname.endswith(".ktestlist"):
            mut_test_map_file_list.append(os.path.join(indir, fname))

    # get ktest to mutant list mapping
    test_to_mutlist = {}
    for minf in mut_test_map_file_list:
        df = pd.read_csv(minf)
        for index, row in df.iterrows():
            assert row["ktest"].endswith(".ktest"), "Invalid minf file: "+minf
            et = row["ellapsedTime(s)"]
            mid = row["MutantID"]
            ktp = os.path.join(indir, row["ktest"])
            if ktp in ktest_list:
                if ktp not in test_to_mutlist:
                    test_to_mutlist[ktp] = []
                test_to_mutlist[ktp].append(mid)
    
    # commpute unit test files and put in the mutant id in comment
    for ktp in ktest_list:
        out_ktp = os.path.join(outdir, os.path.basename(ktp)) + ".c"
        apply_input_to_template(out_ktp, template_file_path, ktp)
        mut_info_str = "/* The following mutants (IDs) were targeted to generated this test: {} */".format(test_to_mutlist[ktp])
        with open(out_ktp) as f:
            test_str = f.read()
        test_str = test_str.replace(
            "/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c */",
            mut_info_str
        )
        with open(out_ktp, "w") as f:
            f.write(test_str)

    if len(ktest_list) == 0:
        print("Warning: No ktest found in ktest dir", indir)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("outdir", help="Directory where to write obtained unittests")
    parser.add_argument("indir", help="Directory containing the .ktests, generated by SEMu")
    parser.add_argument("test_gen_template", help="Test gen template file used to generate the tests")
    args = parser.parse_args()

    outdir = args.outdir
    indir = args.indir
    template_file_path = args.test_gen_template
    assert os.path.isdir(outdir), "outdir is missing"
    assert os.path.isdir(indir), "indir is missing"
    assert os.path.isfile(template_file_path), "template file is missing"
    
    produce_unittests_from_ktest_dir(outdir, indir, template_file_path)


if __name__ == "__main__":
    main()

