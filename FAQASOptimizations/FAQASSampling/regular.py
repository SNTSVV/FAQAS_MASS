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

all_file = str(sys.argv[1])
traces_path = str(sys.argv[2])
rate = float(sys.argv[3])

all_length = file_len(all_file)

mutants = parse_mutants(all_file)
mutant_traces = parse_mutant_traces(traces_path)

print(len(mutant_traces))
for i in range(1,101):
    simulation_name = "sim-" + str(rate) + "/" + str(i) + ".txt"
    print("simulation name: " + simulation_name)

    os.makedirs(os.path.dirname(simulation_name), exist_ok=True) 

    sim_file = open(simulation_name, 'w')
    
    sample_size = math.ceil(rate * all_length)
    print("sample size: " + str(sample_size))
    sampled_mutants = random.sample(mutants, sample_size)
    
    for sampled_mutant in sampled_mutants:
        for trace_line in mutant_traces[sampled_mutant]:
            sim_file.write(trace_line + '\n') 
