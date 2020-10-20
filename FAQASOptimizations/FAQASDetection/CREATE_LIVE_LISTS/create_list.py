import os, sys

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

def print_set(set_to_print, path):
    file_path = open(path, 'w')

    for key, value in set_to_print.items():
        file_path.write(key + '\n')

live_path = str(sys.argv[1])
result_path = os.path.dirname(os.path.realpath(live_path))

traces = load_traces(live_path)
print_set(traces, result_path + '/all_live')
