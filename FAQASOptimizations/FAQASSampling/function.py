import sys, os
from pathlib import Path
import math, random

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

def parse_mutant_traces(traces_path):
    traces = {}
    with open(traces_path, 'r') as f:
        for line in f:
            mutant_trace = line.strip().split(';')
            if mutant_trace[0] + '|' + mutant_trace[1] in traces:
                traces[mutant_trace[0] + '|' + mutant_trace[1]].append(line.strip())
            else:
                traces[mutant_trace[0] + '|' + mutant_trace[1]] = [line.strip()]
    return traces

def organise_traces_per_function(traces):
    traces_per_function = {}
    print(len(traces))
    for key, value in traces.items():
        mutants_fields_w_location = key.split('|')
        location = mutants_fields_w_location[1]
        mutant_fields = mutants_fields_w_location[0].split('.')
        mutant_function_key = mutant_fields[0] + "|" + mutant_fields[5] + "|" + location
        
        if mutant_function_key in traces_per_function:
            traces_per_function[mutant_function_key].append(value)
        else:
            traces_per_function[mutant_function_key] = [value] 

    return traces_per_function
        
all_file = str(sys.argv[1])
traces_path = str(sys.argv[2])
rate = float(sys.argv[3])

mutants = parse_mutants(all_file)
mutant_traces = parse_mutant_traces(traces_path)
f_mutant_traces = organise_traces_per_function(mutant_traces)

print(len(f_mutant_traces))

for i in range(1,101):
    simulation_name = "sim-" + str(rate) + "/" + str(i) + ".txt"
    print("simulation name: " + simulation_name)

    os.makedirs(os.path.dirname(simulation_name), exist_ok=True) 

    sim_file = open(simulation_name, 'w')
    
    for key, value in f_mutant_traces.items():
#        print(key + " " + str(len(value)))
        all_length = len(value)
       
        sample_size = rate * all_length
        
        if sample_size < 1:
            sample_size_ceil = 0
        else:
            sample_size_ceil = math.ceil(sample_size)
        
#        print("sample size: " + str(sample_size_ceil))
        sampled_mutants = random.sample(value, sample_size_ceil)
        
#        print(sampled_mutants)

        for sampled_mutant in sampled_mutants: 
            for trace_line in sampled_mutant:
                sim_file.write(trace_line + '\n') 
