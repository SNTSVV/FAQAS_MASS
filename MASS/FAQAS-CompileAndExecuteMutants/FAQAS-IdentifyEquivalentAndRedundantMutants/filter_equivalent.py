#!/usr/bin/env python3.7

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

import os, sys

equivalents_list = [] # distance zero

def load_equivalents(file_path):
    eq_dict = {}

    with open(file_path) as f:
        for line in f:
            distance_trace = line.strip().split(';')

            if float(distance_trace[3]) == 0.0:
                equivalents_list.append(distance_trace[1] + '|' + distance_trace[0])
            else:
                key = distance_trace[0] + '|' + distance_trace[3]
                value = distance_trace[1] + '|' + distance_trace[0]                

                if key in eq_dict:
                    eq_dict[key].append(value)
                else:
                    eq_dict[key] = [value]
    return eq_dict

def get_list_useful_a(useful_live_dict):
   
    useful_list = []
     
    for key, value in useful_live_dict.items():
        useful_list.append(value.pop())

    return useful_list
def get_list_useful_b(useful_live_dict):
   
    useful_list = []
     
    for key, value in useful_live_dict.items():

        if len(value) > 0:
            for mutant in value:
                useful_list.append(mutant)

    return useful_list

def printList(list_to_print, path):
    file_path = open(path, 'w')

    for mutant in list_to_print:
        file_path.write(mutant + '\n')

if __name__ == '__main__':
    distances_path = str(sys.argv[1])

    useful_live_dict = load_equivalents(distances_path)

    useful_list_a = []
    useful_list_b = []

    useful_list_a = get_list_useful_a(useful_live_dict)
    useful_list_b = get_list_useful_b(useful_live_dict)

    dir_path = os.path.dirname(distances_path)

    
    print("size list A:", len(useful_list_a))
    print("size list B:", len(useful_list_b))
    print("size list equivalents:", len(equivalents_list))


    printList(useful_list_a, os.path.join(dir_path, "useful_list_a"))
    printList(useful_list_b, os.path.join(dir_path, "useful_list_b"))
    printList(equivalents_list, os.path.join(dir_path, "equivalents_distance_zero"))
