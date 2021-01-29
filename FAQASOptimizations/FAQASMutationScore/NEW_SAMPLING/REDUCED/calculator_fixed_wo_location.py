import sys, os
import numpy as np
import pandas as pd
from pathlib import Path

def get_order_tests(test_cases_path):
    original_order = []
    with open(test_cases_path, 'r') as f:
        for line in f:
            original_order.append(line.strip())
    return original_order


def load_traces(file_path):
    traces = {}

    with open(file_path) as f:
        for line in f:
            mutant_trace = line.strip().split(';')
            if mutant_trace[0] in traces:
                traces[mutant_trace[0]].append(line.strip())
            else:
                traces[mutant_trace[0]] = [line.strip()]
    return traces

def get_prioritized_tests(prioritization_path, strategy, mutant_name, mutant_line, iteration):
    tests_list = []

    iteration_str = "iteration_" + str(iteration)

    relative_path = os.path.join(prioritization_path, iteration_str, strategy)
    try:
        result = list(Path(relative_path).rglob(mutant_name + '.c.' + mutant_line + '.prioritized.txt'))[0]
    
        with open(result.absolute()) as f:
            for trace in f:
                trace_fields = trace.split('/')
            
                tests_list.append(trace_fields[4] + ".xml")
    except:
        print(strategy, mutant_name, mutant_line)
                                                                                                                                     
    return tests_list

def define_killed_original(file_path, original_order):
    traces = load_traces(file_path)

    total = len(traces)
    
    result_list = []
    for key, value in traces.items():
        mutant_fields = key.split('.')
        mutant_name = mutant_fields[0]
        mutant_line = mutant_fields[2]

        status = 0
        
        for test in original_order:
            matching = [s for s in value if test in s][0]
            if 'KILLED' in matching:
                status = 1
                break

        result_list.append(status) 
    return result_list

def define_killed(file_path, prioritization_path, test_path, original_order, strategy, iteration):
    traces = load_traces(file_path)

    total = len(traces)
    
    result_list = []
    simulation_counter = 0

    for key, value in traces.items():
        mutant_fields = key.split('.')
        mutant_name = mutant_fields[0]
        mutant_line = mutant_fields[2]

        p_tests = get_prioritized_tests(prioritization_path, strategy, mutant_name, mutant_line, iteration)
        status = 0
        
        if len(p_tests) == 0 or simulation_counter < 100:
            for test in original_order:
                matching = [s for s in value if test in s][0]

                if 'KILLED' in matching:
                    status = 1
                    break
        else:
            for test in p_tests:
                matching_string = test_path + test
                matching = [s for s in value if matching_string in s][0]

                if 'KILLED' in matching:
                    status = 1
                    break
        
        simulation_counter += 1

        result_list.append(status) 
    return result_list

if __name__ == '__main__':
    mutation_result_path = str(sys.argv[1])
    prioritization_path = str(sys.argv[2])
    original_order_path = str(sys.argv[3])
    test_path = str(sys.argv[4])
    
    original_order = get_order_tests(original_order_path)

    sampling_rate = 3000

    ##################
    original_results = []

    for i in range(1, 10 +1):
        sim_file = mutation_result_path + "/sim-" + str(sampling_rate) + "/" + str(i) + ".txt"
        print(sim_file)
        original_results.append(define_killed_original(sim_file, original_order))

        np.set_printoptions(threshold=sys.maxsize)
        matrix_original = np.array(original_results)
        transposed_original = matrix_original.transpose()

        df_original = pd.DataFrame(data=transposed_original.astype(int))
        df_original.to_csv('original.csv', sep=';', header=False, index=False)

    ##################
    
    strat_methods = ['s1jaccard', 's1ochiai', 's2euclidean', 's2cosine']

    for method in strat_methods:
        results = []
        for i in range(1, 10 +1):
            sim_file = mutation_result_path + "/sim-" + str(sampling_rate) + "/" + str(i) + ".txt"
            print(sim_file)
            results.append(define_killed(sim_file, prioritization_path, test_path, original_order, method, i))

            np.set_printoptions(threshold=sys.maxsize)
            matrix = np.array(results)
            transposed = matrix.transpose()

            df = pd.DataFrame(data=transposed.astype(int))
            df.to_csv(method + '.csv', sep=';', header=False, index=False)
