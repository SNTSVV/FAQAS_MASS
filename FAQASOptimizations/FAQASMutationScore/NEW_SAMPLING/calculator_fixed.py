import sys
import numpy as np
import pandas as pd

def load_traces(file_path):
    traces = {}

    with open(file_path) as f:
        for line in f:
            mutant_trace = line.strip().split(';')
            if mutant_trace[0] + '|' + mutant_trace[1] in traces:
                traces[mutant_trace[0] + '|' + mutant_trace[1]].append(line.strip())
            else:
                traces[mutant_trace[0] + '|' + mutant_trace[1]] = [line.strip()]
    return traces
def define_killed(file_path):
    traces = load_traces(file_path)

    total = len(traces)
    
    result_list = []
    for key, value in traces.items():
        status = 0
        for test_execution in value:
            if 'KILLED' in test_execution:
                status = 1
                break
        result_list.append(status)    

    return result_list

if __name__ == '__main__':
    mutation_result_path = str(sys.argv[1])

    sampling_rate = 3000
    results = []

    for i in range(1, 2000 +1):
        sim_file = mutation_result_path + "/sim-" + str(sampling_rate) + "/" + str(i) + ".txt"
        print(sim_file)
        results.append(define_killed(sim_file))

    np.set_printoptions(threshold=sys.maxsize)
    matrix = np.array(results)
    transposed = matrix.transpose()

    df = pd.DataFrame(data=transposed.astype(int))
    df.to_csv('experiments.csv', sep=';', header=False, index=False)
