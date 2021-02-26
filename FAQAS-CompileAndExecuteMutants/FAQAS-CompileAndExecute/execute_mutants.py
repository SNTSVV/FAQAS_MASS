#!/usr/bin/env python3.7                                                                                                              

import os, sys
import argparse
import subprocess
import random

parser = argparse.ArgumentParser()
parser.add_argument('--sampling', type=str)
parser.add_argument('--mut_exec_dir', type=str)
parser.add_argument('--compilation_cmd', type=str)
parser.add_argument('--additional_cmd', type=str)
parser.add_argument('--additional_cmd_after', type=str)
parser.add_argument('--prioritized', type=str)
parser.add_argument('--full_tst_order', type=str)

args = parser.parse_args()

sampling = args.sampling
mut_exec_dir = args.mut_exec_dir
compilation_cmd = args.compilation_cmd
additional_cmd = args.additional_cmd
additional_cmd_after = args.additional_cmd_after
prioritized = args.prioritized
full_tst_order_file = args.full_tst_order

sampled_mutants_file = os.path.join(mut_exec_dir, "sampled_mutants")
traces_mutants_file = os.path.join(mut_exec_dir, "main.csv") 
results_mutants_file = os.path.join(mut_exec_dir, "results.csv")
error_mutants_file = os.path.join(mut_exec_dir, "results_calibration.csv")

cwd = os.path.dirname(os.path.abspath(__file__))
mutation_script = os.path.join(cwd, "mutation.sh")
fsci_stopping_script = os.path.join(cwd, "fsci_stopping_criterion.R")
fsci_prt_stopping_script = os.path.join(cwd, "fsci_prt_stopping_criterion.R")
fsci_error_script = os.path.join(cwd, "fsci_compute_error.R")

# fsci calibration variables
tolerated_error = 0.035
#fsci_calibration = 10
fsci_calibration = 150
delta = 1.0
higher = 0.0
lower = 0.0


def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def fsci_stopping_criterion():
    result = subprocess.run([fsci_stopping_script, results_mutants_file], stdout=subprocess.PIPE).stdout.decode('utf-8')
    print("stopping criterion is", result)
    return int(result)

def fsci_prt_stopping_criterion():
    result = subprocess.run([fsci_prt_stopping_script, results_mutants_file, str(delta), str(lower), str(higher)], stdout=subprocess.PIPE).stdout.decode('utf-8')
    print("stopping criterion is", result)
    return int(result)

def estimate_error():
    result = subprocess.run([fsci_error_script, error_mutants_file, results_mutants_file], stdout=subprocess.PIPE).stdout.decode('utf-8')
    return result

def stopping_criterion(sampling, count):
    if sampling == "fsci":
        if prioritized:
            global delta

            if count < fsci_calibration or delta >= tolerated_error:
                return 0
            else:
                return fsci_prt_stopping_criterion()
        else:
            return fsci_stopping_criterion()
        
    else:
        # we covered all sampled mutants
        total_mutants = file_len(sampled_mutants_file)
        if count == total_mutants:
            return 1
        else: 
            return 0

def load_prioritized():
    prt_dict = {}
    with open(prioritized) as f:
        for line in f:
            prioritized_line = line.strip().split('|')
            key = prioritized_line[0] + '|' + prioritized_line[1]
           
            tst_list = [] 
            for tst in prioritized_line[2].split(';'):
                tst_list.append(tst.split(':')[0])

            prt_dict[key] = tst_list
    return prt_dict

def load_full_ts_list():
    ts_list = []
    with open(full_tst_order_file) as f:
        for line in f:
            ts_list.append(line.strip())
    return ts_list

def log_mutation_result(mutant_file, output):
    print("logging mutation result:", output, "on file:", mutant_file)

    result_file_ = open(mutant_file, 'a+')
    result_file_.write(str(output) + '\n')

def get_prioritized_tests_for_mutant(prt_dict, mutant):
    mutant_fields = mutant.split('|')
    mutant_id_fields = mutant_fields[0].split('.')

    mutant_line_number = mutant_fields[1] + "|" + mutant_id_fields[2]        

    match = [value for key, value in prt_dict.items() if mutant_line_number in key.lower()][0]
    
    return match

def simulate_prioritized(mutant, prt_test_list):

    mutant_fields = mutant.split('|')
    mutant_id = mutant_fields[0] + ';' + mutant_fields[1]

    killed = 0
    with open(traces_mutants_file) as f:
        traces = [line for line in f if mutant_id in line]
      
        for test_case in prt_test_list:
            test_case_key = ";" + test_case + ';'
    
            matches = len([s for s in traces if test_case_key in s and ";KILLED" in s])

            if matches > 0:
                killed = 1

    return killed


def execute_mutants_prioritized_ts():
    prt_dict = load_prioritized()
    
    count = 0
    with open(sampled_mutants_file) as f:
        for line in f:
            mutant = line.strip()

            print(mutant, count)

            match = get_prioritized_tests_for_mutant(prt_dict, mutant)
            mutant_test_list = ";".join(match)
            
            if count < fsci_calibration:
                ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, full_ts_list])
                log_mutation_result(results_mutants_file, ret)  # complete for simulation
                
                prt_ret = simulate_prioritized(mutant, match) # reduced
                log_mutation_result(error_mutants_file, prt_ret)
            
            else:
                if count == fsci_calibration:
                    setFsciError()
               
                global delta
 
                if (delta < tolerated_error):
                    ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, mutant_test_list])
                    log_mutation_result(results_mutants_file, ret)
                else:
                    ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, full_ts_list])
                    log_mutation_result(results_mutants_file, ret)

                    prt_ret = simulate_prioritized(mutant, match) # reduced
                    log_mutation_result(error_mutants_file, prt_ret)
                    
                    setFsciError()
    
            count += 1    
                
            if stopping_criterion(sampling, count) == 1:
                break
        
def setFsciError():
    global delta, lower, higher
    
    error_prt = estimate_error()
    error_prt_split = error_prt.split(";")
    delta = float(error_prt_split[0])
    lower = float(error_prt_split[1])
    higher = float(error_prt_split[2])                    

    print("delta", delta, "lower", lower, "higher", higher)


def execute_mutants_full_ts():
    count = 0

    with open(sampled_mutants_file) as f:
        for line in f:
            mutant = line.strip()

            print(mutant, count)

            ret = subprocess.call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, "full", full_ts_list])
    #        ret = random.randint(0, 1)
            log_mutation_result(results_mutants_file, ret)

            count += 1
            if stopping_criterion(sampling, count) == 1:
                break    

full_ts = load_full_ts_list()
full_ts_list = ";".join(full_ts)

if prioritized:
    execute_mutants_prioritized_ts()
else:
    execute_mutants_full_ts()

