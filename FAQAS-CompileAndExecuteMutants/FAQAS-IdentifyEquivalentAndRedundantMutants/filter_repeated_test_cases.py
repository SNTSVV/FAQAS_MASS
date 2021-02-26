#!/usr/bin/env python3.7

import os, sys

def load_traces(file_path):
    traces = {}

    with open(file_path) as f:
        for line in f:
            mutant_trace = line.strip().split(';')
            key = mutant_trace[1] + '|' + mutant_trace[0]

            if key in traces:
                traces[key].append(line.strip())
            else:
                traces[key] = [line.strip()]
    return traces

def sfilter(traces):
    filtered_traces = []

    for key, value in traces.items():
        highest_distance = 0.0
        trace = ''
        for distance_test in value:
            distance = distance_test.split(';')[3]
            if float(distance) >= float(highest_distance):
                highest_distance = distance
                trace = distance_test
        filtered_traces.append(trace)
    return filtered_traces


if __name__ == '__main__':
    equivalents_path = str(sys.argv[1])

    dir_path = os.path.dirname(equivalents_path)
    equivalents_filename = os.path.basename(equivalents_path)    

    traces = load_traces(equivalents_path)

    result_filename = os.path.join(dir_path, "filtered_" + equivalents_filename)

    result_file = open(result_filename, "w")

    filtered = sfilter(traces)

    for tr in filtered:
        result_file.write(tr + '\n')
