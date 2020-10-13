import sys, os
from pathlib import Path
from divide_killed_live import parse_mutant_traces

def process_original_times(traces, original_order):
    times_dict = {}

    for key, value in traces.items():
        tot_time = 0
    
        for test in original_order:
            matching = [s for s in value if test in s][0]
            matching_fields = matching.split(';')                                                                                            
            curr_time = int(matching_fields[-1])
            tot_time += curr_time
            if 'KILLED' in matching:
                break
        
        times_dict[key] = tot_time
    return times_dict


def print_times(set_to_print, path):
    file_path = open(path, 'w')

    for key, value in set_to_print.items():
        file_path.write(key + ';' + str(value) + '\n')

def get_prioritized_tests(prioritization_path, strategy, mutant_name, mutant_line):
    tests_list = []

    relative_path = os.path.join(prioritization_path, strategy)
    try:
        result = list(Path(relative_path).rglob(mutant_name + '.c.' + mutant_line + '.prioritized.txt'))[0]
    
        with open(result.absolute()) as f:
            for trace in f:
                trace_fields = trace.split('/')
            
                tests_list.append(trace_fields[4])
    except:
        print(strategy, location, mutant_name, mutant_line)
        
    return tests_list

def process_prioritized_times(traces, prioritization_path, test_path, original_order, strategy):
    prioritized_times_dict = {}

    for key, value in traces.items():
        mutant_fields = key.split('.')
        mutant_name = mutant_fields[0]
        mutant_line = mutant_fields[2]

        #print(prioritization_path, strategy, mutant_name, mutant_line)
        p_tests = get_prioritized_tests(prioritization_path, strategy, mutant_name, mutant_line)
        
        tot_time = 0

        for test in p_tests:
            matching_string = test_path + test
            matching = [s for s in value if matching_string in s][0]
            
            matching_fields = matching.split(';')
            curr_time = int(matching_fields[-1])
            tot_time += curr_time
            if 'KILLED' in matching:
                break

        if len(p_tests) == 0:
            for test in original_order:
                matching = [s for s in value if test in s][0]
                matching_fields = matching.split(';')
                curr_time = int(matching_fields[-1])
                tot_time += curr_time
                if 'KILLED' in matching:
                    break

        prioritized_times_dict[key] = tot_time 
    return prioritized_times_dict

def get_order_tests(test_cases_path):
    original_order = []
    with open(test_cases_path, 'r') as f:
        for line in f:
            original_order.append(line.strip())
    return original_order

traces_path = str(sys.argv[1])
prioritization_path = str(sys.argv[2])
original_test_cases = str(sys.argv[3])
test_path = str(sys.argv[4])
 
mutant_traces = parse_mutant_traces(traces_path)
original_order = get_order_tests(original_test_cases)

times_dict = process_original_times(mutant_traces, original_order)
print_times(times_dict, 'times.csv')
strat_methods = ['s1jaccard', 's1ochiai', 's2euclidean', 's2cosine']

for method in strat_methods:

    reduced_set_times_dict = process_prioritized_times(mutant_traces, prioritization_path, test_path, original_order, method)
    print_times(reduced_set_times_dict, method + ".csv") 
