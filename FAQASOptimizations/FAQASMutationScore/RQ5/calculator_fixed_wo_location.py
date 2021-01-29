import sys, os
from pathlib import Path

sampling_rates = [3000]

def get_order_tests(test_cases_path):
    original_order = []
    with open(test_cases_path, 'r') as f:
        for line in f:
            original_order.append(line.strip())
    return original_order

def get_list_of_mutants(list_of_mutants):                                                                                             
    mutants_count = {}
    count = 1
    with open(list_of_mutants) as f:
        for line in f:
            mutants_count[count] = int(line.strip())
            count += 1
    return mutants_count


def load_traces(file_path, mutants_count, TOT, iteration):
    traces = {}

    with open(file_path) as f:
        count = 0
        for line in f:
            mutant_trace = line.strip().split(';')
            if mutant_trace[0] in traces:
                traces[mutant_trace[0]].append(line.strip())
            else:
                traces[mutant_trace[0]] = [line.strip()]

            count += 1
            if count/TOT == mutants_count[iteration]:
                print (mutants_count[iteration])
                break
    return traces


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


def calculate_mutation_score(file_path, prioritization_path, test_path, original_order, strategy, mutants_count, TOT, iteration):
    traces = load_traces(file_path, mutants_count, TOT, iteration) 
    
    total = len(traces)
    
    killed = 0
    timeout = 0

    for key, value in traces.items():
        mutant_fields = key.split('.')
        mutant_name = mutant_fields[0]
        mutant_line = mutant_fields[2]

        p_tests = get_prioritized_tests(prioritization_path, strategy, mutant_name, mutant_line)

        for test in p_tests:
            matching_string = test_path + test
            matching = [s for s in value if matching_string in s][0]
            
            if 'KILLED' in matching:
                killed += 1
                if 'TIMEOUT' in matching:
                    timeout += 1
                break

        if len(p_tests) == 0:
            for test in original_order:
                matching = [s for s in value if test in s][0]
                
                if 'KILLED' in matching:
                    killed += 1
                    if 'TIMEOUT' in matching:
                        timeout += 1
                    break

    return str(total) + ";" + str(killed) + ";" + str(timeout) + ";" + str(total-killed) + ";" + "{:.2f}".format(100*(killed/total))

if __name__ == '__main__':
    mutation_result_path = str(sys.argv[1])
    prioritization_path = str(sys.argv[2])
    original_order_path = str(sys.argv[3])
    test_path = str(sys.argv[4])
    TOT = int(sys.argv[5])
    list_of_mutants = str(sys.argv[6])

    mutants_count = get_list_of_mutants(list_of_mutants) 
    original_order = get_order_tests(original_order_path)

    strat_methods = ['s1jaccard', 's1ochiai', 's2euclidean', 's2cosine']

    for method in strat_methods:

        result_file = open(method + ".csv", "w")

        for sampling_rate in sampling_rates:
            for i in range(1, 100 +1):
                sim_file = mutation_result_path + "/sim-" + str(sampling_rate) + "/" + str(i) + ".txt"
                print(sim_file)
                result = calculate_mutation_score(sim_file, prioritization_path, test_path, original_order, method, mutants_count, TOT, i)
                print(result)
        
                result_file.write(result + "\n")
