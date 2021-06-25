from multiprocessing import Pool
import traceback
import argparse
import numpy
import math
from scipy import spatial
import itertools
import time, sys, os

parser = argparse.ArgumentParser()
parser.add_argument('--original_coverages', type=str)
parser.add_argument('--coverages', type=str)
parser.add_argument('--mutants', type=str)
parser.add_argument('--exec_dir', type=str)
parser.add_argument('--combination', type=str)

args = parser.parse_args()
original_coverages = args.original_coverages
coverages = args.coverages
mutants = args.mutants
exec_dir = args.exec_dir
combination_file = args.combination

coverages_dict = {}
original_coverages_dict = {}

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

def read_combinations(combination_file):
    combinations = []
    with open(combination_file, 'r') as f:
        for line in f:
            mut_a, mut_b = line.strip().split(';')
            combinations.append((mut_a, mut_b))

    return combinations

def read_original_coverages(coverages_path):
    original_coverages = {}

    with open(coverages_path, 'r') as f:
        for line in f:
            test, source, frequencies = line.strip().split(';')
            key = source + '|' + test
        
            original_coverages[key] = frequencies

    return original_coverages

def read_mutants_coverages(coverages_path):
    coverages = {}

    with open(coverages_path, 'r') as f:
        for line in f:
            mutant_coverage = line.strip().split(';')
       
            key = mutant_coverage[0] + '|' + mutant_coverage[1] 
            if key in coverages:
                coverages[key].append(line.strip())
            else:
                coverages[key] = [line.strip()]
    return coverages


def generate_key(mutant, character):
    mutant_data = mutant.split(character)
    mutant_name = mutant_data[0]
    location = mutant_data[1].replace('./', '')

    mutant_source = mutant_name.split('.')[0]

    return location + '/' + mutant_source + '.c' + '|' + mutant_name

def get_source_name(mutant_source):
    mutant, partial_source = mutant_source.split('|')
    partial_source = partial_source.replace('./', '')

    mutant_source = mutant.split('.')[0]

    return partial_source + '/' + mutant_source + '.c'

def retrieve_coverages_mutant(mutant_key, coverages):
    coverage_key = generate_key(mutant_key, '|')
    if coverage_key in coverages:
        return coverages[coverage_key]
    else:
        return []
   
def get_distance(covAList, covBList):

    A = numpy.array(covAList)
    B = numpy.array(covBList)

    distance = cosine(A, B)

    return distance

def correct_coverage(cov_mut, cov_orig, mutated_line, operator, m_name, tst):

    lineNumber = mutated_line - 1

    covMutList = []
    for i in cov_mut:
        if is_int(i):
            covMutList.append(int(i))
        else:
            covMutList.append(int(0))

    covOrigList = []
    for i in cov_orig:
        if is_int(i):
            covOrigList.append(int(i))
        else:
            covOrigList.append(int(0))

    try:
        if operator == "SDL":
            if len(covMutList) == len(covOrigList):
                del covMutList[lineNumber]
                del covOrigList[lineNumber]
            else:
                while len(covMutList) != len(covOrigList):
                    if len(covMutList) > len(covOrigList):
                        del covMutList[lineNumber]
                    else:
                        covMutList.insert(len(covMutList), int(0))
                    del covMutList[lineNumber]
                    del covOrigList[lineNumber]

        elif operator == 'LOD':
             while len(covMutList) != len(covOrigList):
                if len(covMutList) > len(covOrigList):
                    del covMutList[lineNumber]
                else:
                    covMutList.insert(len(covMutList), int(0))
    except (IndexError) as err:
        traceback.print_tb(err.__traceback__)
        print(m_name, tst)
        print(cov_mut)
        print(cov_orig)
        print(lineNumber)
        print(operator)
    
    return (covMutList, covOrigList)

def log(exec_dir, mutant_a, mutant_b, message, test_name, distance):

    if not os.path.exists(exec_dir):
        os.makedirs(exec_dir)
    
    sep = ';'
    result_file = open(exec_dir + "/main.csv", 'a+')
    result_file.write(mutant_a + sep + mutant_b + sep + message + sep + test_name + sep + str(distance) + '\n')
    result_file.close()


def process_combination(combination):
      
    #print("comparing:", combination)

    line_a = int(combination[0].split('.')[2])
    line_b = int(combination[1].split('.')[2])
      
    m_name_a = combination[0].split('|')[0]
    m_name_b = combination[1].split('|')[0]

#    if m_name_a != 'TMFrameBuilder.mut.278.1_1_17.SDL.isTMQueueEmpty':
#        return
#
#    if m_name_b != 'pus3_ExecuteTc_3_129.mut.86.1_1_5.SDL.pus3_ExecuteTc_3_129':
#        return    
 
    op_a = combination[0].split('.')[4]
    op_b = combination[1].split('.')[4]
 
    cov_traces_a = retrieve_coverages_mutant(combination[0], coverages_dict)
    cov_traces_b = retrieve_coverages_mutant(combination[1], coverages_dict)

    # if one of the mutants has no coverage traces, continue
    if len(cov_traces_a) == 0 or len(cov_traces_b) == 0:
        msg = "NO_TESTS"
        log(exec_dir, combination[0], combination[1], msg, "NA", "NA")
        return

    tests_a = []
    tests_b = []

    for trace in cov_traces_a:
        if "NO_COVERAGE_PRODUCED" not in trace:
            tests_a.append(trace.split(';')[2])

    for trace in cov_traces_b:
        if "NO_COVERAGE_PRODUCED" not in trace:
            tests_b.append(trace.split(';')[2])
   
    common_tests = list(set(tests_a).intersection(tests_b)) 

#   print("common tests", common_tests)

    if len(common_tests) == 0:
        msg = "NO_COMMON_TESTS"
        log(exec_dir, combination[0], combination[1], msg, "NA", "NA")
        return

    source_a = get_source_name(combination[0])
    source_b = get_source_name(combination[1])
  
    highest_distance = 0
    selected_tst = ''
    for tst in common_tests:
        cov_a = [s for s in cov_traces_a if tst in s][0].split(';')[3].split(',') 
        cov_b = [s for s in cov_traces_b if tst in s][0].split(';')[3].split(',')   

        cov_a_orig = original_coverages_dict[source_a + '|' + tst].split(',')
        cov_b_orig = original_coverages_dict[source_b + '|' + tst].split(',')


        cor_cov_a, cor_orig_a = correct_coverage(cov_a, cov_a_orig, line_a, op_a, m_name_a, tst)
        cor_cov_b, cor_orig_b = correct_coverage(cov_b, cov_b_orig, line_b, op_b, m_name_b, tst)

        concat_a = cor_cov_a + cor_orig_b
        concat_b = cor_orig_a + cor_cov_b
        
        #print(tst)
        distance = get_distance(concat_a, concat_b)

        #print(distance)
        if distance >= highest_distance:
            highest_distance = distance
            selected_tst = tst

    msg = "DISTANCE"
    log(exec_dir, combination[0], combination[1], msg, selected_tst, highest_distance)

    print(combination[0], combination[1], selected_tst, highest_distance)
    

def process(mutants, coverages, exec_dir):
    
    combinations = read_combinations(combination_file)

    print("total combinations", len(combinations))
    pool = Pool(4)
    pool.map(process_combination, combinations) 
   
#    count = 0
#    for combination in combinations:
#        process_combination(combination)
#        if count == 10:
#            break
#        else:
#            count += 1


if __name__ == '__main__':
   
    original_coverages_dict = read_original_coverages(original_coverages) 
    coverages_dict = read_mutants_coverages(coverages)
    process(mutants, coverages, exec_dir)
