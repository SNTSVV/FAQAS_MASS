import sys, os
from pathlib import Path
import random

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

def process_original_ms(traces, original_order):
    killed = 0
    timeout = 0

    total = len(traces)

    for key, value in traces.items():
        for test in original_order:
            matching = [s for s in value if test in s][0]
            matching_fields = matching.split(';')                                                                                            
            if 'KILLED' in matching:
                killed += 1
                if 'TIMEOUT' in matching:
                    timeout += 1
                break
    return str(total) + ";" + str(killed) + ";" + str(timeout) + ";" + str(total-killed) + ";" + "{:.2f}".format(100*(killed/total))
    
        
def print_times(set_to_print, path):
    file_path = open(path, 'w')

    for key, value in set_to_print.items():
        file_path.write(key + ';' + str(value) + '\n')

def get_prioritized_tests(prioritization_path, strategy, location, mutant_name, mutant_line, iteration):
    tests_list = []

    iteration_str = "iteration_" + str(iteration)

    relative_path = os.path.join(prioritization_path, iteration_str, strategy, location)
    try:
        result = list(Path(relative_path).rglob(mutant_name + '.c.' + mutant_line + '.prioritized.txt'))[0]
    
        with open(result.absolute()) as f:
            for trace in f:
                trace_fields = trace.split('/')
            
                tests_list.append(trace_fields[4])
    except:
        print(strategy, location, mutant_name, mutant_line)
        
    return tests_list

def set_mutant_status(mutant_dict_kl, key, status):
    if key in mutant_dict_kl:
        mutant_dict_kl[key].append(status)
    else:
        mutant_dict_kl[key] = [status]
    

def process_prioritized_ms(traces, prioritization_path, test_path, original_order, strategy, mutant_dict_kl, iteration):
    killed = 0
    timeout = 0

    total = len(traces)
    
    for key, value in traces.items():
        current_killed = 0
        key_fields = key.split('|')
        location = key_fields[1]

        mutant_fields = key_fields[0].split('.')
        mutant_name = mutant_fields[0]
        mutant_line = mutant_fields[2]

        p_tests = get_prioritized_tests(prioritization_path, strategy, location, mutant_name, mutant_line, iteration)
        
        if len(p_tests) == 0:
            
            for test in original_order:
            
                matching = [s for s in value if test in s][0]
                matching_fields = matching.split(';')
                if 'KILLED' in matching:
                    set_mutant_status(mutant_dict_kl, key, 1)
                    killed += 1
                    current_killed = 1
                    if 'TIMEOUT' in matching:
                        timeout += 1
                    break 
        else:
            for test in p_tests:
                matching_string = test_path + test
                matching = [s for s in value if matching_string in s][0]
            
                matching_fields = matching.split(';')
                if 'KILLED' in matching:
                    set_mutant_status(mutant_dict_kl, key, 1)
                    killed += 1
                    current_killed = 1
                    if 'TIMEOUT' in matching:
                        timeout += 1
                    break

        if current_killed == 0:
            set_mutant_status(mutant_dict_kl, key, 0)

    return str(total) + ";" + str(iteration) + ";" + str(killed) + ";" + str(timeout) + ";" + str(total-killed) + ";" + "{:.2f}".format(100*(killed/total))

def get_order_tests(test_cases_path):
    original_order = []
    with open(test_cases_path, 'r') as f:
        for line in f:
            original_order.append(line.strip())
    return original_order

def print_dict(set_to_print, path):
    file_path = open(path, 'w')

    for key, value in set_to_print.items():
        file_path.write(key + ';' + ";".join(str(v) for v in value) + '\n')

def print_ms(ms, path):
    file_path = open(path, 'a+')

    file_path.write(ms + '\n')

traces_path = str(sys.argv[1])
prioritization_path = str(sys.argv[2])
original_test_cases = str(sys.argv[3])
test_path = str(sys.argv[4])
 
mutant_traces = parse_mutant_traces(traces_path)
original_order = get_order_tests(original_test_cases)

original_ms = process_original_ms(mutant_traces, original_order)

print(original_ms)

s1jaccard = {}
s1ochiai = {}
s2cosine = {}
s2euclidean = {}

for iteration in range(1, 11):
    prioritized_ms = process_prioritized_ms(mutant_traces, prioritization_path, test_path, original_order, 's1jaccard', s1jaccard, iteration)
    print(prioritized_ms)
    print_ms(prioritized_ms, "ms_s1jaccard.csv")

print_dict(s1jaccard, "dict_s1jaccard.csv")

for iteration in range(1, 11):
    prioritized_ms = process_prioritized_ms(mutant_traces, prioritization_path, test_path, original_order, 's1ochiai', s1ochiai, iteration)
    print(prioritized_ms)
    print_ms(prioritized_ms, "ms_s1ochiai.csv")

print_dict(s1ochiai, "dict_s1ochiai.csv")     

for iteration in range(1, 11):
    prioritized_ms = process_prioritized_ms(mutant_traces, prioritization_path, test_path, original_order, 's2cosine', s2cosine, iteration)
    print(prioritized_ms)
    print_ms(prioritized_ms, "ms_s2cosine.csv")

print_dict(s2cosine, "dict_s2cosine.csv")     

for iteration in range(1, 11):
    prioritized_ms = process_prioritized_ms(mutant_traces, prioritization_path, test_path, original_order, 's2euclidean', s2euclidean, iteration)
    print(prioritized_ms)
    print_ms(prioritized_ms, "ms_s2euclidean.csv")

print_dict(s2euclidean, "dict_s2euclidean.csv")     
