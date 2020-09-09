import os
import re
import sys
import argparse
import math
import numpy
import operator
from scipy import spatial

parser = argparse.ArgumentParser()
parser.add_argument('--cov_array', nargs='+')
parser.add_argument('--prio', nargs='+')
parser.add_argument('--mut_name', type=str)
parser.add_argument('--strat', type=str)
parser.add_argument('--method', type=str)
parser.add_argument('--casestudy', type=str)
parser.add_argument('--line', type=int)
parser.add_argument('--result', type=str)

args = parser.parse_args()

coverage_array = args.cov_array
prioritized = args.prio
mutantName = args.mut_name
lineNumber = int(args.line)
method = args.method
casestudy = args.casestudy
strategy = args.strat
result_file_path = args.result

distances = {}
            
def getDistanceFromDict(testA, testB):
    if testA < testB:
        distance = distances.get((testA, testB), None)
    else:
        distance = distances.get((testB, testA), None)

    return distance

def searchStringInFile(filename, string):
    fileToSearch = open(filename, 'r')
    for line in fileToSearch:
        if re.search(string, line):
            return line.strip()

def calculateDistance(testA, testB):
    if strategy == 's1':
        distance = get_distance_s1(testA, testB)
    else: 
        distance = get_distance_s2(testA, testB)
    return distance

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

def jaccard(A, B):
    intersection = list(set(A) & set(B))
    union = list(set().union(A,B))
    distance = 1 - (len(intersection)/len(union))

    return distance

def ochiai(A, B):
    intersection = list(set(A) & set(B))
    distance = 1 - (len(intersection) / math.sqrt(len(A) * len(B))) 

    return distance

def getCoverageAsList(test):
    global mutantName
    coverage_line_splitted = searchStringInFile(test, mutantName).split(':')
    coverage = coverage_line_splitted[1]
    
    coverage_frequencies = coverage.split(',')
    return coverage_frequencies

def get_distance_s1(testA, testB):
    
    coverageA_frequencies = getCoverageAsList(testA)
    
    covAList = []
    count = 1
    for i in coverageA_frequencies:
        if is_int(i) and int(i) > 0:
            covAList.append(count)
        count += 1

    coverageB_frequencies = getCoverageAsList(testB)

    covBList = []
    count = 1
    for i in coverageB_frequencies:
        if is_int(i) and int(i) > 0:
            covBList.append(count)
        count += 1
    
    if method == 'jaccard':
        distance = jaccard(covAList, covBList)
    else:
        distance = ochiai(covAList, covBList)

    return distance


def get_distance_s2(testA, testB):
    coverageA_frequencies = getCoverageAsList(testA)

    covAList = []    
    for i in coverageA_frequencies:
        if is_int(i):
            covAList.append(int(i))
        else:
            covAList.append(int(0))
    
    coverageB_frequencies = getCoverageAsList(testB)
    
    covBList = []    
    for i in coverageB_frequencies:
        if is_int(i):
            covBList.append(int(i))
        else:
            covBList.append(int(0))
    
    A = numpy.array(covAList)
    B = numpy.array(covBList)
    
    if method == 'euclidean':
        distance = euclidean(A, B)
    else:
        distance = cosine(A, B)  

    return distance

def save_distance(testA, testB, distance):
    if testA < testB:
            distances[testA,testB] = distance
    else:
        distances[testB,testA] = distance

def determine_most_executed(testList):
    global lineNumber
    
    mostExecuted=''
    count=0
    for test in testList:
        freq = getCoverageAsList(test)
        print(freq)
        if int(freq[lineNumber - 1]) > count:
            mostExecuted = test
    return mostExecuted

def print_most_executed(): 
    directory = os.path.dirname(result_file_path)
    
    if not os.path.exists(directory):
        os.makedirs(directory)

    result_file = open(result_file_path, 'w')
    result_file.write(prioritized[0] + ':0\n')
    result_file.close()

def print_new_test(test, dist_value):
    result_file = open(result_file_path, 'a+')
    result_file.write(test + ':' + str(dist_value) + '\n')
    result_file.close()

print_most_executed()

print("complete set is: " +  str(len(coverage_array)))
# remove most executed from prioritize
coverage_array.remove(prioritized[0])

while len(coverage_array) > 0:
#    print("------ new iteration")
    highest_distances = {}

    for n in coverage_array:
        #n_test = int(re.findall('\d+', n)[0])
        n_test = n.split('/')[4]

        tn=None
        minimal=10000000000
        for t in prioritized:
            #t_test = int(re.findall('\d+', t)[0])
            t_test = t.split('/')[4]
#            print('t is ' + str(t_test) + ' n is ' + str(n_test))

            distance = getDistanceFromDict(t_test, n_test)
            if distance is None:
                distance = calculateDistance(t, n)
                save_distance(t_test, n_test, distance)
                
            if distance < minimal:
                minimal = distance
                tn = n
        
 #       print('min is ' + tn + ' ' + str(minimal))
        highest_distances[tn] = minimal

    max_highest_distance = max(highest_distances.items(), key = operator.itemgetter(1))
    highest_distance = [key for key in highest_distances if highest_distances[key] == max_highest_distance[1]]
    
    if len(highest_distance) == 1:
        dist_value = max_highest_distance[1]
        test = max_highest_distance[0]
    else:
        test = determine_most_executed(highest_distance)
        dist_value = highest_distances[test]
    
    if dist_value == 0:
        break
    else:
#        print(test + ' ' + str(dist_value))
        print_new_test(test, dist_value)
        prioritized.append(test)
        coverage_array.remove(test)

print("reduced set is: " + str(len(prioritized)))

