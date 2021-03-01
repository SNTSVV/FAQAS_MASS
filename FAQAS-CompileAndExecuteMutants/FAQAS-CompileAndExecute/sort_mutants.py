#!/usr/bin/env python3.7

import argparse
import math
import random

parser = argparse.ArgumentParser()
parser.add_argument('--sampling', type=str)
parser.add_argument('--rate', type=str, nargs='?')
parser.add_argument('--all_filtered', type=str)
parser.add_argument('--sampled_mutants', type=str)

args = parser.parse_args()

sampling = args.sampling
rate = float(args.rate)
all_filtered = args.all_filtered
sampled_mutants = args.sampled_mutants

def file_len(fname):                                                                                                                  
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def parse_mutants(fname):
    all_file = open(fname, 'r')
    mutants = []

    for line in all_file:
        mutants.append(line.strip())

    return mutants

def print_sampled_mutants(sampled_mutants_list):
    sampled_mutants_file = open(sampled_mutants, 'a+')
    for mutant in sampled_mutants_list:
        sampled_mutants_file.write(mutant + '\n')
    sampled_mutants_file.close()

def uniform_sampling():
    sample_size = math.ceil(rate * total_mutants)
    sampled_mutants_list = random.sample(all_mutants, sample_size)
    print_sampled_mutants(sampled_mutants_list)

def fsci_sampling():
    sample_size = total_mutants
    sampled_mutants_list = random.sample(all_mutants, sample_size)
    print_sampled_mutants(sampled_mutants_list)

def organise_mutants_per_function(mutants_list):
    mutants_per_function = {}

    for mutant in mutants_list:
        mutants_fields_w_location = mutant.split('|')
        location = mutants_fields_w_location[1]
        mutant_fields = mutants_fields_w_location[0].split('.')
        mutant_function_key = mutant_fields[0] + "|" + mutant_fields[5] + "|" + location

        if mutant_function_key in mutants_per_function:
            mutants_per_function[mutant_function_key].append(mutant)
        else:
            mutants_per_function[mutant_function_key] = [mutant] 

    return mutants_per_function

def stratified_sampling():
    dict_mutants_f = organise_mutants_per_function(all_mutants)

    for key, value in dict_mutants_f.items():
        all_length = len(value)
       
        sample_size = rate * all_length
        
        if sample_size < 1:
            sample_size_ceil = 0
        else:
            sample_size_ceil = math.ceil(sample_size)

        sampled_mutants_list = random.sample(value, sample_size_ceil)
        print_sampled_mutants(sampled_mutants_list)

all_mutants = parse_mutants(all_filtered)
total_mutants = file_len(all_filtered)

print("Sorting mutants using sampling:", sampling)

if sampling == 'uniform':
    uniform_sampling()
elif sampling == 'stratified':
    stratified_sampling()
else:
    fsci_sampling()


