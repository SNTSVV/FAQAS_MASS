import re
import math
from scipy import spatial

def print_new_test(result, dist_value):
    result_file = open(result, 'a+')
    result_file.write(str(dist_value))
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
