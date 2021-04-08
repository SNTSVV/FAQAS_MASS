import sys

def load_traces(file_path):
    traces = {}

    with open(file_path) as f:
        for line in f:
            mutant_trace = line.strip().split(';')
            if  mutant_trace[1] in traces:
                traces[mutant_trace[1]].append(line.strip())
            else:
                traces[mutant_trace[1]] = [line.strip()]
    return traces

def sfilter(traces):
    filtered_traces = []

    for key, value in traces.items():
        highest_distance = 0.0
        trace = ''
        for distance_test in value:
            distance = distance_test.split(';')[4]
            print(key, "d: ", distance, "h:", highest_distance)
            if float(distance) >= float(highest_distance):
                highest_distance = distance
                trace = distance_test
        filtered_traces.append(trace)
    return filtered_traces
            

if __name__ == '__main__':
    equivalents_path = str(sys.argv[1])

    traces = load_traces(equivalents_path)

    result_file = open("filtered_" + equivalents_path, "w")
    
    filtered = sfilter(traces) 

    for tr in filtered:
        result_file.write(tr + '\n')
