import os, sys

def parse_mutant_traces(fname):                                                                                                  
    traces = {}
    with open(fname, 'r') as f:
        for line in f:
            if 'STUBBORN' in line:
                print(line)
                continue

            mutant_trace = line.strip().split(';')
            if mutant_trace[0] in traces:
                traces[mutant_trace[0]].append(line.strip())
            else:
                traces[mutant_trace[0]] = [line.strip()]
    return traces

def get_all_red_eq(fname):
    red_eq_file = open(fname, 'r')
    mutants = []

    for line in red_eq_file: 
        mutants.append(line.strip())
    return mutants

red_eq = sys.argv[1]
all_traces = sys.argv[2]

all_traces_dict = parse_mutant_traces(all_traces)
red_eq_list = get_all_red_eq(red_eq)

print(len(all_traces_dict))
print(len(red_eq_list))

for mutant in red_eq_list:
    if mutant in all_traces_dict:
        del all_traces_dict[mutant]


all_filtered = open('all_filtered', 'w')
all_traces_filtered = open('all_traces_filtered.csv', 'w')

for key, value in all_traces_dict.items():
    all_filtered.write(key + '\n')

    for trace in value:
        all_traces_filtered.write(trace + '\n')            

print(len(all_traces_dict))

