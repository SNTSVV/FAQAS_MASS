import sys, os
from pathlib import Path

def parse_mutant_traces(traces_path):                                                                                                           
    traces = {}
    with open(traces_path, 'r') as f:
        count = 0
        traces_read = 0
        for line in f:
            mutant_trace = line.strip().split(';')
            if mutant_trace[0] + '|' + mutant_trace[1] in traces:
                traces[mutant_trace[0] + '|' + mutant_trace[1]].append(line.strip())
            else:
                traces[mutant_trace[0] + '|' + mutant_trace[1]] = [line.strip()]
    return traces

def process_original_tests(traces, original_order):
    tests_dict = {}

    tot_tests = 0
    for key, value in traces.items():
        for test in original_order:
            matching = [s for s in value if test in s][0]
            matching_fields = matching.split(';')                                                                                            
            tot_tests += 1
            if 'KILLED' in matching:
                break
        
        tests_dict[1] = tot_tests
    return tests_dict


def print_tests(set_to_print, path):
    file_path = open(path, 'w')

    for key, value in set_to_print.items():
        file_path.write(str(key) + ';' + str(value) + '\n')

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


def process_prioritized_tests(traces, prioritization_path, test_path, original_order, strategy, mutant_dict_kl, iteration):

    print(strategy, iteration)
    tot_tests = 0
    
    for key, value in traces.items():
        key_fields = key.split('|')
        location = key_fields[1]

        mutant_fields = key_fields[0].split('.')
        mutant_name = mutant_fields[0]
        mutant_line = mutant_fields[2]

        p_tests = get_prioritized_tests(prioritization_path, strategy, location, mutant_name, mutant_line, iteration)
        
        for test in p_tests:
            matching_string = test_path + test
            matching = [s for s in value if matching_string in s][0]
            
            matching_fields = matching.split(';')
            tot_tests += 1
            if 'KILLED' in matching:
                break

        if len(p_tests) == 0:
            for test in original_order:
                matching = [s for s in value if test in s][0]
                matching_fields = matching.split(';')
                tot_tests += 1 
                if 'KILLED' in matching:
                    break

    mutant_dict_kl[iteration] = tot_tests 

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

tests_dict = process_original_tests(mutant_traces, original_order)
print_tests(tests_dict, 'tests.csv')

s1jaccard = {}
s1ochiai = {}
s2cosine = {}
s2euclidean = {}

for iteration in range(1, 11):
    process_prioritized_tests(mutant_traces, prioritization_path, test_path, original_order, 's1jaccard', s1jaccard, iteration)

print_tests(s1jaccard, "dict_s1jaccard.csv")

for iteration in range(1, 11):
    process_prioritized_tests(mutant_traces, prioritization_path, test_path, original_order, 's1ochiai', s1ochiai, iteration)

print_tests(s1ochiai, "dict_s1ochiai.csv")

for iteration in range(1, 11):
    process_prioritized_tests(mutant_traces, prioritization_path, test_path, original_order, 's2cosine', s2cosine, iteration)

print_tests(s2cosine, "dict_s2cosine.csv")

for iteration in range(1, 11):
    process_prioritized_tests(mutant_traces, prioritization_path, test_path, original_order, 's2euclidean', s2euclidean, iteration)

print_tests(s2euclidean, "dict_s2euclidean.csv")

