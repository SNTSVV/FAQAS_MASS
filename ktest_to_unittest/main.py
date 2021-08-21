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

    result_str = ""
    
    #Create result from template, using the data

    # Write result
    with open(result_file_path, "w") as f:
        f.write(result_str)
