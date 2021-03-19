#! /usr/bin/env python

"""
Requires Python 3
Use muteria for this (update muteria on pip before)
TODO
"""

import os
import argparse
import shutil

from jinja2 import Template

import muteria.configmanager.configurations as configurations
import muteria.configmanager.helper as configs_helper
from muteria.controller.main_controller import MainController

TOOLNAME = "FAQAS_SEMu"

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_metamu_bitcode_file", 
                        help="Meta mutant LLVM bitcode file")
    parser.add_argument("--mutants_list_file", 
                        help="optional file containing the list of "
                                "mutants to consider")
    parser.add_argument("--output_top_directory", 
                        help="Directory where to put the output "
                            + "(as {}-out). ".format(TOOLNAME)
                            + "The bitcode location is used by default.")
    parser.add_argument("--clear_existing", action="store_true"
                        help="Clear any existing output, instead of failing")
    args = parser.parse_args()
    return args

def call_underlying_semu(conf_file):
    lang = "c"
    cfg_obj = configs_helper.ConfigurationHelper()
    raw_conf = cfg_obj.get_extend_file_raw_conf(conf_file, lang)
    raw_conf['EXECUTION_CLEANSTART'] = True
    raw_conf['RUN_MODE'] = configurations.SessionMode.EXECUTE_MODE

    ctrl = MainController()
    ctrl.raw_config_main(raw_config=cmd_raw_conf)
    


TEST_GENERATION_TIMEOUT = 7200 #2h

def main():
    """
    Entrypoint
    """
    # useful
    test_gen_muteria_conf_template = os.path.join(
                                os.path.dirname(os.path.realpath(__file__)), 
                                "muteria_tg_conf.py.template"
    )
    
    # get comand line args
    args = get_args()
    input_metamu_bitcode_file = args.input_metamu_bitcode_file
    assert os.path.isfile(input_metamu_bitcode_file), \
            "The specified input metamu bitcode file does not exist!"
    if args.output_top_directory is not None:
        output_top_directory = args.output_top_directory
        assert os.path.isdir(output_top_directory), \
                "The specified output top directory does not exist!"
    else:
        output_top_directory = os.path.dirname(input_metamu_bitcode_file)
    output_directory = os.path.join(output_top_directory, 
                                    "{}-out".format(TOOLNAME))
    if os.path.isdir(output_directory):
        if args.clear_existing:
            shutil.rmtree(output_directory)
        else:
            assert False, "The output directory {} already exist".format(\
                                                            output_directory)

    mutants_list_file = args.mutants_list_file
    if args.mutants_list_file is not None:
        assert os.path.isfile(mutants_list_file), \
                "The specified mutant list file does not exist"
    
    temporary_workdir = os.path.join(output_directory, "todelete_{}.tmp")
    os.mkdir(temporary_workdir)

    test_gen_muteria_conf = os.path.join(temporary_workdir, 
                                            "tmp_muteria_tg_conf.py")

    # generate the muteria conf from template
    ## Load template
    with open(test_gen_muteria_conf_template) as ft:
        template_str = f.read()

    ## compute parameters and apply to resolve template (TODO)
    ### REPODIR, OUTDIR, 
    muteria_output = os.path.join(temporary_workdir, "output")
    resolved_conf = Template(template_str).render({
        "template_repo_rootdir": os.path.join(temporary_workdir, "repo"),
        "template_output_dir": muteria_output,
        "template_programe_name": os.path.splitext(os.path.basename(input_metamu_bitcode_file)),
        "tempate_test_gen_maxtime": TEST_GENERATION_TIMEOUT,
        "candidate_mutants_list": mutants_list_file
    })

    ## write the resolved config
    with open(test_gen_muteria_conf, "w") as f:
        f.write(resolved_conf)
    
    # Call test generation using resolved conf
    call_underlying_semu(test_gen_muteria_conf)

    # move the generated tests (TODO) 
    os.mkdir(output_directory)

    shutil.move(os.path.join(muteria_output, 'latest', 'testgeneration', 'semu'), output_directory)

    # Delete temporary files' folder
    shutil.rmtree(temporary_workdir)

    print("# @{}: Test generation is Completed".format(TOOLNAME))
#~ def main()

if __name__ == "__main__":
    main()
