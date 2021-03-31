#!/usr/bin/env python3.7

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

import argparse
import numpy
from utilities import print_new_test, is_int, cosine, euclidean, searchStringInFile 

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

    distance = cosine(A, B)
    print(distance)

    return distance

distance = get_distance(cov_a, cov_b)
print_new_test(result, distance)

