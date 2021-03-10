#!/usr/bin/env python3.7                                                                                                              

import os, sys
import argparse
import subprocess
from mass_sampling import MASSSampling

parser = argparse.ArgumentParser()
parser.add_argument('--sampling', type=str)
parser.add_argument('--mut_exec_dir', type=str)
parser.add_argument('--compilation_cmd', type=str)
parser.add_argument('--additional_cmd', type=str)
parser.add_argument('--additional_cmd_after', type=str)
parser.add_argument('--prioritized', type=str)
parser.add_argument('--reduced', type=str, nargs='?')
parser.add_argument('--timeout', type=str)

args = parser.parse_args()

sampling = args.sampling
mut_exec_dir = args.mut_exec_dir
compilation_cmd = args.compilation_cmd
additional_cmd = args.additional_cmd
additional_cmd_after = args.additional_cmd_after
prioritized = args.prioritized
reduced = args.reduced
timeout = args.timeout

sampled_mutants_file = os.path.join(mut_exec_dir, "sampled_mutants")
traces_mutants_file = os.path.join(mut_exec_dir, "main.csv") 
results_mutants_file = os.path.join(mut_exec_dir, "results.csv")
error_mutants_file = os.path.join(mut_exec_dir, "results_calibration.csv")

cwd = os.path.dirname(os.path.abspath(__file__))
mutation_script = os.path.join(cwd, "mutation.sh")

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

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


def execute_mutants_full_ts():
    prt_dict = load_test_set(prioritized)

    mass_sampling = MASSSampling(results_mutants_file, sampling, file_len(sampled_mutants_file), reduced)

    count = 0 

    with open(sampled_mutants_file) as f:
        for line in f:
            mutant = line.strip()

            print(mutant, count)

            prt_match = get_tests_for_mutant(prt_dict, mutant)
            prt_mutant_test_list = ";".join(prt_match)

            ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, prt_mutant_test_list, timeout])
            log_mutation_result(results_mutants_file, ret)

            count += 1
            if mass_sampling.stopping_criterion(count) == 1:
                break    

def execute_mutants_reduced_ts():
    
    fsci = MASSSampling(results_mutants_file, sampling, file_len(sampled_mutants_file), reduced)

    prt_dict = load_test_set(prioritized)
    red_dict = load_test_set(reduced)
    
    count = 0
    with open(sampled_mutants_file) as f:
        for line in f:
            mutant = line.strip()

            print(mutant, count)

            prt_match = get_tests_for_mutant(prt_dict, mutant)
            prt_mutant_test_list = ";".join(prt_match)

            red_match = get_tests_for_mutant(red_dict, mutant)
            red_mutant_test_list = ";".join(red_match)
            
            if count < fsci.fsci_calibration:
                ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, prt_mutant_test_list, timeout])
                log_mutation_result(results_mutants_file, ret)  # complete for simulation
                
                red_ret = simulate_reduced(mutant, red_match) # reduced
                log_mutation_result(error_mutants_file, red_ret)
            
            else:
                if count == fsci.fsci_calibration:
                    fsci.setFsciError(error_mutants_file)
               
                if (fsci.delta < fsci.tolerated_error):
                    ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, red_mutant_test_list, timeout])
                    log_mutation_result(results_mutants_file, ret)
                else:
                    ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, prt_mutant_test_list, timeout])
                    log_mutation_result(results_mutants_file, ret)

                    red_ret = simulate_reduced(mutant, red_match) # reduced
                    log_mutation_result(error_mutants_file, red_ret)
                    
                    fsci.setFsciError(error_mutants_file)
    
            count += 1    
                
            if fsci.stopping_criterion(count) == 1:
                break

if __name__ == '__main__':

    if reduced:
        execute_mutants_reduced_ts()
    else:
        execute_mutants_full_ts()

