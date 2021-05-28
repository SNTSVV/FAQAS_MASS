#!/usr/bin/env python3.6

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

import os, sys
import random
from collections import OrderedDict

equivalents_list = [] # distance zero

def load_equivalents(file_path):
    eq_list = []

    with open(file_path, encoding='utf-8') as f:
        for line in f:
            source, mutant, test, distance, time = line.strip().split(';')
    
            mutant_tuple = (source, mutant, float(distance))

            if float(distance) == 0.0:
                equivalents_list.append(mutant_tuple)
            else:
                eq_list.append(mutant_tuple)

    return sorted(eq_list, key = lambda tup: tup[2], reverse=True) 

def diff_distance(mutant_t1, mutant_t2):
    return abs(mutant_t1[2] - mutant_t2[2])


def stratify(ordered_mutants, structure):
    strat_dict = {}

    for mutant in ordered_mutants:
        line = mutant[1].split('.')[2]
        function = mutant[1].split('.')[5]
        
        source = mutant[0]

        if structure == 'lines':
            key = source + '|' + line

        elif structure == 'functions':
            key = source + '|' + function

        if key in strat_dict:
            strat_dict[key].append(mutant)
        else:
            strat_dict[key] = [mutant]

    return strat_dict  


def strategy_s1(ordered_mutants):
    listA = []
    listB = []

    while len(ordered_mutants) > 0:  

        if len(listA) == 0:
            listA.append(ordered_mutants.pop(0))
        else:
            higher_distance = -1
            higher_mutant = ()

            last_mutant_listA = listA[-1]

            for mutant in list(ordered_mutants):
                diff = diff_distance(last_mutant_listA, mutant)
                
                if diff == 0:
                    listB.append(mutant)
                    ordered_mutants.remove(mutant)

                elif diff > higher_distance:
                    higher_mutant = mutant
                    higher_distance = diff

            if higher_distance > 0:
                listA.append(higher_mutant)
                ordered_mutants.remove(higher_mutant)          
          
    return (listA, listB)

def strategy_s2(ordered_mutants):

    priority_lists = {}

    dict_lines = stratify(ordered_mutants, 'lines')
   
    print(len(dict_lines))
 
    priority = 1

    while len(dict_lines) > 0:
        print("priority", priority)
        new_list = []

    
        for key, value in list(dict_lines.items()):
            if len(value) == 0:
                del dict_lines[key]
            else:
                new_list.append(value.pop(0))
       
        if len(new_list) > 0:
            print("len", len(new_list)) 
            priority_lists[priority] = new_list
            priority += 1

    print(len(priority_lists))
    return priority_lists

def strategy_s3(ordered_mutants):

    priority_lists = {}

    dict_lines = stratify(ordered_mutants, 'functions')

    print(len(dict_lines))

    priority = 1

    while len(dict_lines) > 0:
        print("priority", priority)
        new_list = []


        for key, value in list(dict_lines.items()):
            if len(value) == 0:
                del dict_lines[key]
            else:
                new_list.append(value.pop(0))

        if len(new_list) > 0:
            print("len", len(new_list))
            priority_lists[priority] = new_list
            priority += 1

    print(len(priority_lists))
    return priority_lists

def strategy_s4(ordered_mutants):

    random_lists = {}
   
    random.shuffle(ordered_mutants)

    list_nr = 1 
    while len(ordered_mutants) > 0:
        random_list = ordered_mutants[0:100]
        del ordered_mutants[0:100]
 
        random_lists[list_nr] = random_list
        list_nr += 1

    return random_lists
        
def printList(list_to_print, path):
    file_path = open(path, 'w', encoding='utf-8')

    for mutant in list_to_print:
        file_path.write(mutant[1] + ';' + mutant[0] + ';' + str(mutant[2]) + '\n')

if __name__ == '__main__':
    distances_path = str(sys.argv[1])
    strategy = str(sys.argv[2])

    mutants = load_equivalents(distances_path)

    dir_path = os.path.dirname(distances_path)
    
    printList(equivalents_list, os.path.join(dir_path, "equivalents_distance_zero.csv"))

    if strategy == 's1':
        listA, listB = strategy_s1(mutants)
    
    
        print("size list A:", len(listA))
        print("size list B:", len(listB))
        print("size list equivalents:", len(equivalents_list))

        printList(listA, os.path.join(dir_path, "useful_list_a.csv"))
        printList(listB, os.path.join(dir_path, "useful_list_b.csv"))

    elif strategy == 's2':
        priority_lists = strategy_s2(mutants) 

        for key, value in priority_lists.items():
            printList(value, os.path.join(dir_path, "useful_list_lines_" + str(key) + ".csv"))
    
    elif strategy == 's3':
        priority_lists = strategy_s3(mutants) 

        for key, value in priority_lists.items():
            printList(value, os.path.join(dir_path, "useful_list_functions_" + str(key) + ".csv"))

    elif strategy == 's4':
        random_lists = strategy_s4(mutants)

        for key, value in random_lists.items():
            printList(value, os.path.join(dir_path, "random_list_" + str(key) + ".csv"))
