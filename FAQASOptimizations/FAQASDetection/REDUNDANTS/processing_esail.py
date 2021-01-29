import argparse
import numpy
import math
from scipy import spatial
import itertools
import time, sys, os

parser = argparse.ArgumentParser()
parser.add_argument('--coverages', type=str)
parser.add_argument('--mutants', type=str)
parser.add_argument('--exec_dir', type=str)
parser.add_argument('--mut_traces', type=str)
parser.add_argument('--test_path', type=str)

args = parser.parse_args()
coverages = args.coverages
mutants = args.mutants
exec_dir = args.exec_dir
mut_traces = args.mut_traces
test_path = args.test_path

redundants = [] 

def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def euclidean(v1, v2):
    distance = [(a - b)**2 for a, b in zip(v1, v2)]
    distance = math.sqrt(sum(distance))

    return distance

def cosine(v1, v2):
    return spatial.distance.cosine(v1, v2)

def read_mutants(mutants, mutant_key):
    mutant_list = []
    with open(mutants, 'r') as f:
        for line in f:
            mutant_data = line.strip().split('|')
            mutant_name = mutant_data[0]
            location = mutant_data[1].replace('./', '')

            mutant_source_name = mutant_name.split('.')[0] + '.'
            mutant_source = location + '/' + mutant_source_name + 'c'

            if mutant_source_name in mutant_key and location in mutant_key:            
                mutant_list.append(line.strip())
    return mutant_list

def read_coverages(coverages_path):
    coverages = {}

    with open(coverages_path, 'r') as f:
        for line in f:
            mutant_coverage = line.strip().split(';')
        
            if mutant_coverage[0] + '|' + mutant_coverage[1] in coverages:
                coverages[mutant_coverage[0] + '|' + mutant_coverage[1]].append(line.strip())
            else:
                coverages[mutant_coverage[0] + '|' + mutant_coverage[1]] = [line.strip()]
    return coverages

def read_mutant_traces(traces_path, mutants_list):
    traces = {}
    with open(traces_path, 'r') as f:
        for line in f:
            mutant_fields = line.strip().split(';')  
            mutant_key = mutant_fields[0] + '|' + mutant_fields[1]

            if mutant_key not in mutants_list:
                continue

            if mutant_key in traces:
                traces[mutant_key].append(line.strip())
            else:
                traces[mutant_key] = [line.strip()]
    return traces 

def generate_key(mutant, character):
    mutant_data = mutant.split(character)
    mutant_name = mutant_data[0]
    location = mutant_data[1].replace('./', '')

    mutant_source = mutant_name.split('.')[0]

    return location + '/' + mutant_source + '.c' + '|' + mutant_name

def retrieve_coverages_mutant(mutant_key, coverages):
    coverage_key = generate_key(mutant_key, '|')
    if coverage_key in coverages:
        return coverages[coverage_key]
    else:
        return []
   
def retrieve_traces_mutant(mutant_key, traces, common_tests, test_path):
    traces_mutant_test = []
    for trace in traces[mutant_key]:
        executed_test = trace.split(';')[3].replace(test_path, "") + "/"
        if executed_test in common_tests:
            traces_mutant_test.append(trace)

    return traces_mutant_test

def common_killed_tests(traces_a, traces_b):

    tests_a = []
    tests_b = []
    for trace in traces_a:
        if "KILLED" in trace:
            tests_a.append(trace.split(';')[3])
    for trace in traces_b:
        if "KILLED" in trace:
            tests_b.append(trace.split(';')[3])

    print("killed tests:", tests_a, tests_b)
    if set(tests_a) == set(tests_b):
        return True
    else:
        return False
    
def get_distance(coverageA_frequencies, coverageB_frequencies, lineNumber_a, lineNumber_b):
    covAList = []
    for i in coverageA_frequencies:
        if is_int(i):
            covAList.append(int(i))
        else:
            covAList.append(int(0))

    covBList = []
    for i in coverageB_frequencies:
        if is_int(i):
            covBList.append(int(i))
        else:
            covBList.append(int(0))

    if len(covAList) != len(covBList):
        while len(covAList) != len(covBList):
            if len(covAList) > len(covBList):
                covBList.insert(lineNumber_b, int(0))
            else:
                covAList.insert(lineNumber_a, int(0))

    A = numpy.array(covAList)
    B = numpy.array(covBList)

    distance = cosine(A, B)

    return distance

def log(exec_dir, mutant_a, mutant_b, message, test_name, distance):

    if not os.path.exists(exec_dir):
        os.makedirs(exec_dir)
    
    sep = ';'
    result_file = open(exec_dir + "/main.csv", 'a+')
    result_file.write(mutant_a + sep + mutant_b + sep + message + sep + test_name + sep + str(distance) + '\n')
    result_file.close()


def process(mutants, coverages, test_path, exec_dir):
    
    coverages_dict = read_coverages(coverages)
  
    key = next(iter(coverages_dict))
    mutants_list = read_mutants(mutants, key)
    mutants_traces_dict = read_mutant_traces(mut_traces, mutants_list)

    combinations = list(itertools.combinations(mutants_list, 2)) 

    print("total mutants", len(mutants_list))
    print("total combinations", len(combinations))

    count = 0
    for combination in combinations:

        if combination[0] in redundants or combination[1] in redundants:
            continue

        print("comparing:", combination)

        line_a = int(combination[0].split('.')[2])
        line_b = int(combination[1].split('.')[2])
        
        cov_traces_a = retrieve_coverages_mutant(combination[0], coverages_dict)
        cov_traces_b = retrieve_coverages_mutant(combination[1], coverages_dict)

        if len(cov_traces_a) == 0 or len(cov_traces_b) == 0:
            msg = "NR_NO_COMMON_TESTS"
            log(exec_dir, combination[0], combination[1], msg, "NA", "NA")
            continue

        tests_a = []
        tests_b = []

        for trace in cov_traces_a:
            if "NO_COVERAGE_PRODUCED" not in trace:
                tests_a.append(trace.split(';')[2])

        for trace in cov_traces_b:
            if "NO_COVERAGE_PRODUCED" not in trace:
                tests_b.append(trace.split(';')[2])

        if len(tests_a) == 0 and len(tests_b) == 0 and line_a == line_b:
            msg = "REDUNDANTS_NO_COV_A_NOR_B"
            log(exec_dir, combination[0], combination[1], msg, "NA", "NA") 
            redundants.append(combination[1])
            continue

        elif len(tests_a) == 0 and len(tests_b) == 0 and line_a != line_b:
            msg = "NR_NO_COMMON_TESTS"
            log(exec_dir, combination[0], combination[1], msg, "NA", "NA")
            continue
   
        common_tests = list(set(tests_a).intersection(tests_b)) 

        print("common tests", common_tests)

        if len(common_tests) == 0:
            msg = "NR_NO_COMMON_TESTS"
            log(exec_dir, combination[0], combination[1], msg, "NA", "NA")
            continue

    
        mut_traces_a = retrieve_traces_mutant(combination[0], mutants_traces_dict, common_tests, test_path)
        mut_traces_b = retrieve_traces_mutant(combination[1], mutants_traces_dict, common_tests, test_path)
        
        if not common_killed_tests(mut_traces_a, mut_traces_b):
            msg = "NR_NO_COMMON__KILLED_TESTS"
            log(exec_dir, combination[0], combination[1], msg, "NA", "NA")
            continue

#        print(cov_traces_a)
#        print(cov_traces_b)

        highest_distance = 0
        selected_tst = ''
        for tst in common_tests:
            cov_a = [s for s in cov_traces_a if tst in s][0].split(';')[3].split(',') 
            cov_b = [s for s in cov_traces_b if tst in s][0].split(';')[3].split(',')   

#            print("compare", cov_a, cov_b)

            distance = get_distance(cov_a, cov_b, line_a, line_b)

            if distance >= highest_distance:
                highest_distance = distance
                selected_tst = tst

        msg = "DISTANCE"
        log(exec_dir, combination[0], combination[1], msg, selected_tst, distance)

        print(combination[0], combination[1], selected_tst, distance)

#        if count == 2:
#            break
#        else:
#            count += 1

def print_redundants(redundants, exec_dir):
    result_file = open(exec_dir + "/redundants", 'a+')

    for red in redundants:
        result_file.write(red + '\n')

    result_file.close()        


process(mutants, coverages, test_path, exec_dir)
print_redundants(redundants, exec_dir)
