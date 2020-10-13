import os, sys

live = {}
killed = {}

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

if __name__ == '__main__':
    traces_path = str(sys.argv[1])  


    mutant_traces = parse_mutant_traces(traces_path)

    print(len(mutant_traces))

    divide_per_status(mutant_traces)

    path = os.path.dirname(os.path.realpath(traces_path))
    print(path)
    printSet(killed, path + "/all_filtered_traces_killed.csv")
    printSet(live, path + "/all_filtered_traces_live.csv")

    print(len(live))
    print(len(killed))
