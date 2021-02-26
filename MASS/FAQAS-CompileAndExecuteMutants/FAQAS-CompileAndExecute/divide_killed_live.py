#!/usr/bin/env python3.7

import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--traces', type=str)

args = parser.parse_args()
traces_path = args.traces

live = {}
killed = {}

def parse_mutant_traces(traces_path):
    traces = {}
    with open(traces_path, 'r') as f:
        for line in f:
            mutant_trace = line.strip().split(';')
            key = mutant_trace[0] + '|' + mutant_trace[1]
            if key in traces:
                traces[key].append(line.strip())
            else:
                traces[key] = [line.strip()]
    return traces

def add_mutant_to_killed(key, value):
    global killed
    killed[key] = value

def add_mutant_to_live(key,value):
    global live
    live[key] = value

def divide_per_status(mutant_traces):
    for key, value in mutant_traces.items():

        killed_mutant = False
        for trace in value:
            if 'KILLED' in trace:
                killed_mutant = True
                break

        if killed_mutant:
            add_mutant_to_killed(key, value)
        else:
            add_mutant_to_live(key, value)

def printSet(set_to_print, path):
    file_path = open(path, 'w')

    for key, value in set_to_print.items():
        for trace in value:
            file_path.write(trace + '\n')

mutant_traces = parse_mutant_traces(traces_path)
print("total mutant traces:", len(mutant_traces))

divide_per_status(mutant_traces)

path = os.path.dirname(os.path.realpath(traces_path))
printSet(killed, path + "/traces_killed.csv")
printSet(live, path + "/traces_live.csv")

