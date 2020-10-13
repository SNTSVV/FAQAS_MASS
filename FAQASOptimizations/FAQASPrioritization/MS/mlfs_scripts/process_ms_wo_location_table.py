import sys, os
from pathlib import Path

def parse_mutant_traces(traces_path):
    traces = {}
    with open(traces_path, 'r') as f:
        for line in f:
            mutant_trace = line.strip().split(';')
            if mutant_trace[0] in traces:
                traces[mutant_trace[0]].append(line.strip())
            else:
                traces[mutant_trace[0]] = [line.strip()]
    return traces

def process_original_ms(traces, original_order, mutant_dict_kl):
    killed = 0
    timeout = 0

#    mutant_dict_kl = {}

    total = len(traces)

    print (total)
    for key, value in traces.items():
        current_killed = 0
        for test in original_order:
            #print(key)
            #print([s for s in value if test in s])
            matching = [s for s in value if test in s][0]
            matching_fields = matching.split(';')                                                                                            
            if 'KILLED' in matching:
                mutant_dict_kl[key] = 1
                killed += 1
                current_killed = 1
                if 'TIMEOUT' in matching:
                    timeout += 1
                break
        if current_killed == 0:
            mutant_dict_kl[key] = 0

    return str(total) + ";" + str(killed) + ";" + str(timeout) + ";" + str(total-killed) + ";" + "{:.2f}".format(100*(killed/total))
    
        
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
        print(strategy, mutant_name, mutant_line)
        
    return tests_list

def process_prioritized_ms(traces, prioritization_path, test_path, original_order, strategy, mutant_dict_kl):

    killed_set = []
    live_set = []
    
#    mutant_dict_kl = {}
    
    killed = 0
    timeout = 0

    total = len(traces)
    
    for key, value in traces.items():
        current_killed = 0

        mutant_fields = key.split('.')
        mutant_name = mutant_fields[0]
        mutant_line = mutant_fields[2]

#        print(prioritization_path, strategy, mutant_name, mutant_line)
        p_tests = get_prioritized_tests(prioritization_path, strategy, mutant_name, mutant_line)
        
        if len(p_tests) == 0:
            for test in original_order:
                matching = [s for s in value if test in s][0]
                matching_fields = matching.split(';')
                if 'KILLED' in matching:
                    mutant_dict_kl[key] = 1
                    killed_set.append(key)
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
                    mutant_dict_kl[key] = 1
                    killed_set.append(key)
                    killed += 1
                    current_killed = 1
                    if 'TIMEOUT' in matching:
                        timeout += 1
                    break

        if current_killed == 0:
            mutant_dict_kl[key] = 0
            live_set.append(key)

#    printSet(killed_set, "killed_" + strategy + ".csv")
#    printSet(live_set, "live_" + strategy + ".csv")
#    printDict(mutant_dict_kl, "dict_" + strategy + ".csv")
    
    return str(total) + ";" + str(killed) + ";" + str(timeout) + ";" + str(total-killed) + ";" + "{:.2f}".format(100*(killed/total))

def get_order_tests(test_cases_path):
    original_order = []
    with open(test_cases_path, 'r') as f:
        for line in f:
            original_order.append(line.strip())
    return original_order

#def printSet(set_to_print, path):
#    file_path = open(path, 'w')
#
#    for value in set_to_print:
#        file_path.write(value + '\n')

def printDict(original_kl, s1jaccard, s1ochiai, s2euclidean, s2cosine):
    file_path = open('result.csv', 'w')

    sep = ';'

    file_path.write('mutant;full;s1jaccard;s1ochiai;s2euclidean;s2cosine\n')
    for key, value in original_kl.items():
        file_path.write(key + sep + str(value) + sep + str(s1jaccard[key]) + sep + str(s1ochiai[key]) + sep + str(s2euclidean[key]) + sep + str(s2cosine[key]) + '\n')


traces_path = str(sys.argv[1])
prioritization_path = str(sys.argv[2])
original_test_cases = str(sys.argv[3])
test_path = str(sys.argv[4])
 
mutant_traces = parse_mutant_traces(traces_path)
original_order = get_order_tests(original_test_cases)

original_kl = {}
original_ms = process_original_ms(mutant_traces, original_order, original_kl)
print(original_ms)

s1jaccard = {}
s1ochiai = {}
s2cosine = {}
s2euclidean = {}

prioritized_ms = process_prioritized_ms(mutant_traces, prioritization_path, test_path, original_order, 's1jaccard', s1jaccard)
print(prioritized_ms)

prioritized_ms = process_prioritized_ms(mutant_traces, prioritization_path, test_path, original_order, 's1ochiai', s1ochiai)
print(prioritized_ms)

prioritized_ms = process_prioritized_ms(mutant_traces, prioritization_path, test_path, original_order, 's2euclidean', s2euclidean)
print(prioritized_ms)

prioritized_ms = process_prioritized_ms(mutant_traces, prioritization_path, test_path, original_order, 's2cosine', s2cosine)
print(prioritized_ms)

printDict(original_kl, s1jaccard, s1ochiai, s2euclidean, s2cosine)

