import os
import binascii
import struct
import argparse

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
        test_str.replace(
            "/* Append this to the generate meta-mu source code to create the <name>.MetaMu.MakeSym.c*/",
            mut_info_str
        )
        with open(out_ktp, "w") as f:
            f.write(test_str)


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

