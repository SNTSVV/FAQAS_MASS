#! /usr/bin/env python

"""
Requires Python 3
Use muteria for this (update muteria on pip before)
TODO
"""

import os
import argparse
import shutil
import json

from jinja2 import Template

import muteria.configmanager.configurations as configurations
import muteria.configmanager.helper as configs_helper
from muteria.controller.main_controller import MainController
import muteria.common.mix as common_mix

TOOLNAME = "FAQAS_SEMu"

TEST_GENERATION_TIMEOUT = 7200 #2h

MEMORY_LIMIT = 2000 # MB

# FIXME: add use_MDO
CONFIGS = {
    "FULL": {
        "template_PL": "0",
        "template_CW": "4294967295",
        "template_MPD": "0",
        "template_PP": "1.0",
        "template_NTPM": "5",
        "template_PSS": "RND" # alternative value is 'MDO'
    },
    "PARTIAL": {
        "template_PL": "0",
        "template_CW": "2",
        "template_MPD": "2",
        "template_PP": "0.5",
        "template_NTPM": "5",
        "template_PSS": "RND" # alternative value is 'MDO'
    }
}

PARAM_NAME_PREFIX = "template_"
DEFAULT_CONFIG = json.dumps({k[len(PARAM_NAME_PREFIX):]: v for k,v in CONFIGS["FULL"].items()})

def compute_config_from_str(json_str):
    obj = json.loads(json_str)
    obj = {PARAM_NAME_PREFIX+k: v for k, v in obj.items()}
    missing = [k[len(PARAM_NAME_PREFIX):] for k in set(CONFIGS["FULL"]) - set(obj)]
    extra = [k[len(PARAM_NAME_PREFIX):] for k in set(obj) - set(CONFIGS["FULL"])]
    assert len(missing) == 0, "The following semu parameter are missing in semu heuristic config: {}".format(missing)
    assert len(extra) == 0, "The following specified parameter, in semu heuristic config, are not supported by semu: {}".format(extra)
    return obj


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
    parser.add_argument("--clear_existing", action="store_true",
                        help="Clear any existing output, instead of failing")
    parser.add_argument("--enable_post_mutation_check", action="store_true",
                        help="Enable post mutation check (may not be complete with loops)")
    parser.add_argument("--symbolic_args", default=None,
                        help="optional symbolic args as string (this enables the system tests mode)")
    parser.add_argument("--generation_timeout", default=TEST_GENERATION_TIMEOUT, type=int,
                        help="test generation timeout in seconds ( > 0).")
    parser.add_argument("--semu_heuristics_config", default=DEFAULT_CONFIG, type=str, #choices=list(CONFIGS),
                        help="Configuration for test generation. must be a string of {}.".format(list(CONFIGS)))

    parser.add_argument("--max_memory_MB", default=MEMORY_LIMIT, type=int,
                        help="memory limit (in MB) of the test generation (> 0).")
    parser.add_argument("--stop_on_memory_limit", action="store_true",
                        help="abort the test generation when the memory limit is exceeded)")
    parser.add_argument("--max_memory_inhibit", action="store_true",
                        help="Stop forking new states on condition when the max memory is reached")
    args = parser.parse_args()
    return args

def call_underlying_semu(conf_file):
    lang = "c"
    cfg_obj = configs_helper.ConfigurationHelper()
    raw_conf = cfg_obj.get_extend_file_raw_conf(conf_file, lang)
    raw_conf['EXECUTION_CLEANSTART'] = True
    raw_conf['RUN_MODE'] = configurations.SessionMode.EXECUTE_MODE

    ce_backup = common_mix.confirm_execution
    common_mix.confirm_execution = lambda _: True
    ctrl = MainController()
    ctrl.raw_config_main(raw_config=raw_conf)
    common_mix.confirm_execution = ce_backup
    


def main():
    """
    Entrypoint
    """
    # useful
    test_gen_muteria_conf_template = os.path.join(
                                os.path.dirname(os.path.realpath(__file__)), 
                                "muteria_tg_conf.template.py"
    )
    
    # get comand line args
    args = get_args()
    input_metamu_bitcode_file = args.input_metamu_bitcode_file
    assert os.path.isfile(input_metamu_bitcode_file), \
            "The specified input metamu bitcode file does not exist ({})!".format(input_metamu_bitcode_file)
    if args.output_top_directory is not None:
        output_top_directory = args.output_top_directory
        assert os.path.isdir(output_top_directory), \
                "The specified output top directory does not exist ({})!".format(output_top_directory)
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
        mutants_list_file = "'" + mutants_list_file + "'"

    enable_post_mutation_check = args.enable_post_mutation_check
    
    symbolic_args = args.symbolic_args
    if symbolic_args is None:
        sym_args_list_of_lists = []
    else:
        #sym_args_list_of_lists = [('-sym-args', '2', '2', '2')]
        sym_args_list_of_lists = [tuple(arg_group.split()) for arg_group in symbolic_args.split(",")]

    generation_timeout = args.generation_timeout
    assert generation_timeout > 0, "generation timeout must be > 0."

    semu_config = args.semu_heuristics_config

    memory_limit = args.max_memory_MB
    assert memory_limit > 0, "memory limit must be > 0."

    stop_on_memory_limit = args.stop_on_memory_limit
    max_memory_inhibit = args.max_memory_inhibit

    temporary_workdir = os.path.join(output_directory, "todelete_{}.tmp".format("work"))
    os.makedirs(temporary_workdir)

    test_gen_muteria_conf = os.path.join(temporary_workdir, 
                                            "tmp_muteria_tg_conf.py")

    # generate the muteria conf from template
    ## Load template
    with open(test_gen_muteria_conf_template) as ft:
        template_str = ft.read()

    ## compute parameters and apply to resolve template (TODO)
    ### REPODIR, OUTDIR, 
    muteria_output = os.path.join(temporary_workdir, "output")
    template_data = {
        "template_repo_rootdir": os.path.join(temporary_workdir, "repo"),
        "template_output_dir": muteria_output,
        "template_programe_name": os.path.splitext(os.path.basename(input_metamu_bitcode_file))[0],
        "template_test_gen_maxtime": generation_timeout,
        "template_candidate_mutants_list": mutants_list_file,
        "template_disable_post_mutation_check": (not enable_post_mutation_check),
        "template_sym_args_list_of_lists": sym_args_list_of_lists,
        "template_meta_mu_bc_file": input_metamu_bitcode_file,
        "template_memory_limit": memory_limit,
        "template_stop_on_memory_limit": stop_on_memory_limit,
        "template_max_memory_inhibit": max_memory_inhibit
    }
    template_data.update(compute_config_from_str(semu_config))
    resolved_conf = Template(template_str).render(template_data)

    ## write the resolved config
    with open(test_gen_muteria_conf, "w") as f:
        f.write(resolved_conf)
    
    # Call test generation using resolved conf
    call_underlying_semu(test_gen_muteria_conf)

    # move the generated tests (TODO) 
    #os.mkdir(output_directory)
    shutil.move(os.path.join(muteria_output, 'latest', 'testscases_workdir', 'semu'), output_directory)

    # Delete temporary files' folder
    shutil.rmtree(temporary_workdir)

    print("# @{}: Test generation is Completed".format(TOOLNAME))
#~ def main()

if __name__ == "__main__":
    main()
