import argparse
import numpy
import time, sys, os
import itertools

parser = argparse.ArgumentParser()
parser.add_argument('--original_coverages', type=str)
parser.add_argument('--coverages', type=str)
parser.add_argument('--mutants', type=str)
parser.add_argument('--tests', type=str)
parser.add_argument('--exec_dir', type=str)
parser.add_argument('--combination', type=str)

args = parser.parse_args()
original_coverages = args.original_coverages
coverages = args.coverages
mutants = args.mutants
tests = args.tests
exec_dir = args.exec_dir

coverages_dict = {}
original_coverages_dict = {}

lines_dict = {}

def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def read_original_coverages(coverages_path):
    original_coverages = {}

    with open(coverages_path, 'r') as f:
        for line in f:
            test, source, frequencies = line.strip().split(';')
            key = source + '|' + test.replace('/', '')
        
            coverage_frequencies = frequencies.split(',')

            original_coverages[key] = coverage_frequencies

            if source not in lines_dict:
                lines_dict[source] = len(coverage_frequencies)

    return original_coverages

def read_test_list(tests_path):
    test_list = []
    
    with open(tests_path, 'r') as f:
        for line in f:
            test_list.append(line.strip())

    return test_list

def read_mutant_list(mutants_path):
    mutant_list = []
    
    with open(mutants_path, 'r') as f:
        for line in f:
            mutant_list.append(line.strip())

    return mutant_list

def read_mutants_coverages(coverages_path):
    coverages = {}

    with open(coverages_path, 'r') as f:
        for line in f:
            mutant_coverage = line.strip().split(';')
       
            key = mutant_coverage[0] + '|' + mutant_coverage[1] + '|' + mutant_coverage[2].replace('/', '')
 
            coverages[key] = mutant_coverage[3].split(',')
    return coverages


def get_source_name(mutant_source):
    mutant, partial_source = mutant_source.split('|')
    partial_source = partial_source.replace('./', '')

    mutant_source = mutant.split('.')[0]

    return partial_source + '/' + mutant_source + '.c'
   
def correct_coverage(covMutList, covOrigList, lineNumber, operator):

    if operator == "SDL" or operator == "LOD":
        if len(covMutList) != len(covOrigList):
            while len(covMutList) != len(covOrigList):
                if len(covMutList) > len(covOrigList):
                    del covMutList[lineNumber]
                else:
                    covMutList.insert(len(covMutList), int(0))

    return covMutList

def prepare_coverage(coverage):
    processed_coverage = []

    for i in coverage:
        if is_int(i):
            processed_coverage.append(int(i))
        else:
            processed_coverage.append(int(0))

    return processed_coverage
    
def get_mutant_name(mutant_key):
    return mutant_key.split('|')[0]

def process(original_coverages_dict, coverages_dict, mutant_list, test_list):
  
    a_file = open(exec_dir + "/cosa.txt", "w") 

    count = 0
    print("generating table...") 
    for mutant in mutant_list:
        start_time = time.time()

        print(mutant)
        mutant_source = get_source_name(mutant)
        mutant_name = get_mutant_name(mutant)

        mutant_coverage_dict = {}
        i = 0
        
        for test in test_list:
            for source in lines_dict:
                
                source_key = source + '|' + test

                if source_key in original_coverages_dict:
                    original_coverage = prepare_coverage(original_coverages_dict[source_key])
                else:
                    original_coverage = [0] * lines_dict[source]

                s = 0
                if source == mutant_source:
                    mutant_key = mutant_source + '|' + mutant_name + '|' + test
    
                    if mutant_key in coverages_dict:
                        mutated_source, mut, line, position, operator, function = mutant.split('|')[0].split('.')
    
                        mutant_coverage = prepare_coverage(coverages_dict[mutant_key])

                        cor_mutant_coverage = correct_coverage(mutant_coverage, original_coverage, int(line) - 1, operator)

                        mutant_coverage_dict[i] = cor_mutant_coverage
                    else:
                        mutant_coverage_dict[i] = original_coverage
                
                else:
                    mutant_coverage_dict[i] = original_coverage
                i += 1

        mutant_coverage_list = numpy.hstack(list(mutant_coverage_dict.values()))

        numpy.savetxt(a_file, mutant_coverage_list, delimiter=',') 

        print(len(mutant_coverage_list), "elapsed:", time.time() - start_time)   
        count +=1

        if count == 2:
            break
    a_file.close()


if __name__ == '__main__':
   
    original_coverages_dict = read_original_coverages(original_coverages) 
    coverages_dict = read_mutants_coverages(coverages)
    mutant_list = read_mutant_list(mutants)
    test_list = read_test_list(tests)

    process(original_coverages_dict, coverages_dict, mutant_list, test_list)

