#!/usr/bin/env python3.7                                                                                                              

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

import os, sys
import argparse
import subprocess
import random

parser = argparse.ArgumentParser()
parser.add_argument('--sampling', type=str)
parser.add_argument('--mutant_id', type=str)
parser.add_argument('--mut_exec_dir', type=str)
parser.add_argument('--compilation_cmd', type=str)
parser.add_argument('--additional_cmd', type=str)
parser.add_argument('--additional_cmd_after', type=str)
parser.add_argument('--prioritized', type=str)
parser.add_argument('--reduced', type=str, nargs='?')
parser.add_argument('--timeout', type=str)
parser.add_argument('--reduced_execution', type=str)

args = parser.parse_args()

sampling = args.sampling
mutant = args.mutant_id
mut_exec_dir = args.mut_exec_dir
compilation_cmd = args.compilation_cmd
additional_cmd = args.additional_cmd
additional_cmd_after = args.additional_cmd_after
prioritized = args.prioritized
reduced = args.reduced
timeout = args.timeout
reduced_execution = args.reduced_execution

traces_mutants_file = os.path.join(mut_exec_dir, "main.csv") 
results_mutants_file = os.path.join(mut_exec_dir, "results.csv")
error_mutants_file = os.path.join(mut_exec_dir, "results_calibration.csv")

cwd = os.path.dirname(os.path.abspath(__file__))
mutation_script = os.path.join(cwd, "mutation.sh") if os.environ['BUILD_SYSTEM'] == 'Makefile' else os.path.join(cwd, "mutation_waf.sh")

def load_test_set(set_csv):
    prt_dict = {}
    with open(set_csv) as f:
        for line in f:
            prioritized_line = line.strip().split('|')
            key = prioritized_line[0] + '|' + prioritized_line[1]
           
            tst_list = [] 
            for tst in prioritized_line[2].split(';'):
                tst_list.append(tst.split(':')[0])

            prt_dict[key] = tst_list
    return prt_dict

def log_mutation_result(mutant_file, output):
    print("logging mutation result:", output, "on file:", mutant_file)

    result_file_ = open(mutant_file, 'a+')
    result_file_.write(str(output) + '\n')

def get_tests_for_mutant(prt_dict, mutant):
    mutant_fields = mutant.split('|')
    mutant_id_fields = mutant_fields[0].split('.')

    mutant_line_number = mutant_fields[1] + "|" + mutant_id_fields[2]        

    match = [value for key, value in prt_dict.items() if mutant_line_number in key.lower()][0]
    
    return match

def simulate_reduced(mutant, test_list):

    mutant_fields = mutant.split('|')
    mutant_id = mutant_fields[0] + ';' + mutant_fields[1]

    killed = 0
    with open(traces_mutants_file) as f:
        traces = [line for line in f if mutant_id in line]
      
        for test_case in test_list:
            test_case_key = ";" + test_case + ';'
    
            matches = len([s for s in traces if test_case_key in s and ";KILLED" in s])

            if matches > 0:
                killed = 1

    return killed


def execute_mutants_reduced_ts():
    red_dict = load_test_set(reduced)

    red_match = get_tests_for_mutant(red_dict, mutant)
    red_mutant_test_list = ";".join(red_match)

    ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, red_mutant_test_list, timeout])
    log_mutation_result(results_mutants_file, ret)


def execute_mutants_full_ts():
    prt_dict = load_test_set(prioritized)
    red_dict = load_test_set(reduced)
    
    prt_match = get_tests_for_mutant(prt_dict, mutant)
    prt_mutant_test_list = ";".join(prt_match)

    red_match = get_tests_for_mutant(red_dict, mutant)
    red_mutant_test_list = ";".join(red_match)
            
    if sampling == "fsci":
        ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, prt_mutant_test_list, timeout])
        log_mutation_result(results_mutants_file, ret)  # complete for simulation
                
        red_ret = simulate_reduced(mutant, red_match) # reduced
        log_mutation_result(error_mutants_file, red_ret)
    else:
        ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, prt_mutant_test_list, timeout])
        log_mutation_result(results_mutants_file, ret)


if reduced_execution == "true":
    execute_mutants_reduced_ts()
else:
    execute_mutants_full_ts()

