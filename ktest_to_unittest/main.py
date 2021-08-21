import os
import binascii
import struct

from muteria.drivers.testgeneration.testcase_formats.ktest.utils import ConvertCollectKtestsSeeds

def extract_input_from_ktest(path_to_ktest_file):
    path_to, ktest_file = os.path.split(path_to_ktest_file)

    c_obj = ConvertCollectKtestsSeeds()
    _, dat = c_obj._loadAndGetSymArgsFromKleeKTests([ktest_file], path_to, must_have_model_version=False)

    hex_res = {}
    int_res = {}
    uint_res = {}
    for name, val in dat['KTEST-OBJ'][0].objects:
        name = name.decode('UTF-8', 'backslashreplace')
        hexval = binascii.hexlify(val).decode('ascii')
        intval = None
        uintval = None
        for n, m in [(1, 'b'), (2, 'h'), (4, 'i'), (8, 'q')]:
            if len(val) == n:
                intval = struct.unpack(m, val)[0])
                uintval = struct.unpack(m.upper(), val)[0])
                break
        hex_res[name] = hexval
        int_res[name] = intval
        uint_res[name] = uintval
    
    assert len(hex_res) % 2 == 0, "Error, invalid hex_res (non even len)"
    hex_array_res = ["0x"+hex_res[i:i+2] for i in range(0, len(hex_res), 2)]

    return hex_array_res, hex_res, int_res, uint_res


def apply_input_to_template(result_file_path, template_file_path, path_to_ktest_file):
    hex_array_res, hex_res, int_res, uint_res = extract_input_from_ktest(path_to_ktest_file)

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
        dat_decl.append("    const unsigned char *" + dat_name + " = {" + ", ".join(hex_array_res[name]) + "};")
        ### replace klee_make_symbolics with memcpy
        mcpy = tmp[0].replace("klee_make_symbolic(", "memcpy(") + ", " + dat_name + tmp[1] + ");"
        result_lines_list[kms] = mcpy

    ## 3. Delete missing input klee_make_symbolic and add hex array for existing inputs
    missing_inputs_pos.sort(reverse=True)
    klee_make_sym_locs.sort()
    for pos in missing_inputs_pos:
        del result_lines_list[pos]
    result_lines_list[klee_make_sym_locs[0]: klee_make_sym_locs[0]] = data_decl

    # Write result
    with open(result_file_path, "w") as f:
        f.write("\n".join(result_lines_list))
