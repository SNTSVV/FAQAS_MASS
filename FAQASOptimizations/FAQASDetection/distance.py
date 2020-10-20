import argparse
import re
import numpy
import math
from scipy import spatial

parser = argparse.ArgumentParser()
parser.add_argument('--name', type=str)
parser.add_argument('--cov_a', type=str)
parser.add_argument('--cov_b', type=str)
parser.add_argument('--result', type=str)
parser.add_argument('--line', type=int) 

args = parser.parse_args()

cov_a = args.cov_a
cov_b = args.cov_b
name = args.name
result = args.result
lineNumber = int(args.line)

def print_new_test(dist_value): 
    global result                                                                                                 
    result_file = open(result, 'a+')
    result_file.write(cov_a + ';' + cov_b + ';'  + str(dist_value))
    result_file.close()

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

def searchStringInFile(filename, string):                                                                                            
    fileToSearch = open(filename, 'r')
    for line in fileToSearch:
        if re.search(string, line):
            return line.strip()
    return ""

def getCoverageAsList(test):                                                                                                         
    global name
    coverage_line_splitted = searchStringInFile(test, name).split(':')
    
    if len(coverage_line_splitted) == 1:
        raise ValueError("coverage not found")
    
    coverage = coverage_line_splitted[1]

    coverage_frequencies = coverage.split(',')
    return coverage_frequencies

def get_distance(testA, testB):
    try:                                                                                                   
        coverageA_frequencies = getCoverageAsList(testA)
        coverageB_frequencies = getCoverageAsList(testB)
    except ValueError as err:
        print(err.args)
        return -1

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
        global lineNumber
        while len(covAList) != len(covBList):
            if len(covAList) > len(covBList):
                covBList.insert(lineNumber, int(0))
            else:
                covAList.insert(lineNumber, int(0))
    print(covAList) 
    print(covBList) 

    A = numpy.array(covAList)
    B = numpy.array(covBList)

    distance = euclidean(A, B)
    print(distance)

    return distance

distance = get_distance(cov_a, cov_b)
print_new_test(distance)

