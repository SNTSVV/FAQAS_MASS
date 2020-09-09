import sys

sampling_rates = [0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]

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

def calculate_mutation_score(file_path):
    traces = load_traces(file_path) 
    
    total = len(traces)
    
    killed = 0
    timeout = 0
    
    for key, value in traces.items():
        for test_execution in value:
            if 'KILLED' in test_execution:
                killed += 1
                if 'TIMEOUT' in test_execution:
                    timeout += 1
                break
    return str(total) + ";" + str(killed) + ";" + str(timeout) + ";" + str(total-killed) + ";" + "{:.2f}".format(100*(killed/total))

if __name__ == '__main__':
    mutation_result_path = str(sys.argv[1])

    result_file = open("simulations.csv", "w")

    for sampling_rate in sampling_rates:
        for i in range(1, 100 +1):
            sim_file = mutation_result_path + "/sim-" + str(sampling_rate) + "/" + str(i) + ".txt"
            print(sim_file)
            result = str(sampling_rate) + ";" + str(i) + ";" + calculate_mutation_score(sim_file)
            print(result)
        
            result_file.write(result + "\n")
