import os, sys

def parse_mutant_traces(fname):                                                                                                  
    traces = {}
    with open(fname, 'r') as f:
        for line in f:
            mutant_trace = line.strip().split(';')
            if mutant_trace[0] + '|' + mutant_trace[1] in traces:
                traces[mutant_trace[0] + '|' + mutant_trace[1]].append(line.strip())
            else:
                traces[mutant_trace[0] + '|' + mutant_trace[1]] = [line.strip()]
    return traces

def get_all_red_eq(fname):
    red_eq_file = open(fname, 'r')
    mutants = []

    for line in red_eq_file: 
        mutants.append(line.strip())
    return mutants

def remove_stubborns(all_traces_dict):
    stubborns = []

    for key, value in all_traces_dict.items():
        for trace in value:
            if 'STUBBORN' in trace:
                stubborns.append(key)

    for stubborn in stubborns:
        del all_traces_dict[stubborn]

    return all_traces_dict

def remove_red_eq(all_traces_dict, red_eq_list):
    for mutant in red_eq_list:
        if mutant in all_traces_dict:
            del all_traces_dict[mutant]
    
    return all_traces_dict

red_eq = sys.argv[1]
all_traces = sys.argv[2]

all_traces_dict = parse_mutant_traces(all_traces)
red_eq_list = get_all_red_eq(red_eq)

print(len(all_traces_dict))
print(len(red_eq_list))

all_traces_filtered = remove_red_eq(all_traces_dict, red_eq_list)
all_traces_filtered_stubborns = remove_stubborns(all_traces_filtered)

all_filtered = open('all_filtered', 'w')
all_traces_filtered = open('all_traces_filtered.csv', 'w')

for key, value in all_traces_filtered_stubborns.items():
    all_filtered.write(key + '\n')

    for trace in value:
        all_traces_filtered.write(trace + '\n')            

print(len(all_traces_filtered_stubborns))

