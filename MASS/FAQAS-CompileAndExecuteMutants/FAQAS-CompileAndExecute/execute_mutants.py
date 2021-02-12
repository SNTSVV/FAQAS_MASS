#!/usr/bin/env python3.7                                                                                                              

import os, sys
import argparse
from subprocess import call

parser = argparse.ArgumentParser()
parser.add_argument('--sampling', type=str)
parser.add_argument('--mut_exec_dir', type=str)
parser.add_argument('--compilation_cmd', type=str)
parser.add_argument('--additional_cmd', type=str)
parser.add_argument('--additional_cmd_after', type=str)
parser.add_argument('--prioritized', type=str)

args = parser.parse_args()

sampling = args.sampling
mut_exec_dir = args.mut_exec_dir
compilation_cmd = args.compilation_cmd
additional_cmd = args.additional_cmd
additional_cmd_after = args.additional_cmd_after
prioritized = args.prioritized

sampled_mutants_file = os.path.join(mut_exec_dir, "sampled_mutants")

cwd = os.path.dirname(os.path.abspath(__file__))
mutation_script = os.path.join(cwd, "mutation.sh")

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def stopping_criterion(sampling, count):
    if sampling == "fsci":
        None
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

prt_dict = load_prioritized()

count = 0        
with open(sampled_mutants_file) as f:
    for line in f:
        mutant = line.strip()

        mutant_fields = mutant.split('|')
        mutant_id_fields = mutant_fields[0].split('.')

        mutant_line_number = mutant_fields[1] + "|" + mutant_id_fields[2]        

        match = [value for key, value in prt_dict.items() if mutant_line_number in key.lower()][0]
        
        mutant_test_list = ";".join(match)
        
        call([mutation_script, mut_exec_dir, mutant, compilation_cmd, additional_cmd, additional_cmd_after, mutant_test_list])
        count += 1    
        if stopping_criterion(sampling, count) == 1:
            break
    
