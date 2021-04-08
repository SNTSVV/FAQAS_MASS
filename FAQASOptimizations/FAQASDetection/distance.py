import argparse
import numpy
from utilities import print_new_test, is_int, cosine, euclidean, searchStringInFile 

parser = argparse.ArgumentParser()
parser.add_argument('--name', type=str)
parser.add_argument('--cov_a', type=str)
parser.add_argument('--cov_b', type=str)
parser.add_argument('--result', type=str)
parser.add_argument('--line', type=int) 
parser.add_argument('--operator', type=str)

args = parser.parse_args()

cov_a = args.cov_a
cov_b = args.cov_b
name = args.name
result = args.result
operator = args.operator
lineNumber = int(args.line) - 1

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


    # SDL, LOD corrections
    global lineNumber

    if operator == "SDL":
        if len(covAList) == len(covBList):
            del covAList[lineNumber]
            del covBList[lineNumber]
        else:
            while len(covAList) != len(covBList):
                if len(covAList) > len(covBList):
                    del covAList[lineNumber]
                else:
                    covAList.insert(len(covAList), int(0))
                del covAList[lineNumber]
                del covBList[lineNumber]

    elif operator == 'LOD':
         while len(covAList) != len(covBList):
            if len(covAList) > len(covBList):
                del covAList[lineNumber]
            else:
                covAList.insert(len(covAList), int(0))            
   
    print(covAList) 
    print(covBList) 

    A = numpy.array(covAList)
    B = numpy.array(covBList)

    distance = cosine(A, B)
    print(distance)

    return distance

distance = get_distance(cov_a, cov_b)
print_new_test(result, distance)

