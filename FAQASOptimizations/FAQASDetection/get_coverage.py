import argparse
import numpy
from utilities import print_new_test, is_int, searchStringInFile 

parser = argparse.ArgumentParser()
parser.add_argument('--name', type=str)
parser.add_argument('--cov', type=str)
parser.add_argument('--result', type=str)

args = parser.parse_args()

cov = args.cov
name = args.name
result = args.result

def getCoverageAsList(test):                                                                                                         
    global name
    coverage_line_splitted = searchStringInFile(test, name).split(':')
    
    if len(coverage_line_splitted) == 1:
        raise ValueError("coverage not found")
    
    coverage = coverage_line_splitted[1]

    return coverage

distance = getCoverageAsList(cov)
print_new_test(result, distance)

