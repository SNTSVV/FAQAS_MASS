
#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

import csv
import sys

raw_results = sys.argv[1]
mutants_table = sys.argv[2]
readable_results = sys.argv[3]

raw_has_header = False

lookup_map = {}

lookup_map["empty"] = ['NA'] * 11

with open(str(mutants_table), 'r') as map:
    linecount = 0
    map_reader = csv.reader(map, delimiter=',')
    for definition in map_reader:
        # cancelUnusedParameters(definition)
        mutant = definition.pop(0)
        if linecount == 0:
            mutant = "header"
        lookup_map[str(mutant)] = definition
        linecount += 1

with open(str(raw_results), 'r') as raw, open(str(readable_results), 'w') as results:
    results.truncate(0)
    raw_reader = csv.reader(raw, delimiter=';')
    results_writer = csv.writer(results, delimiter=',')

    linecount = 0

    for row in raw_reader:
        mutant = row[0]
        if raw_has_header and linecount == 0:
            mutant = "header"
        new_row = row
        if mutant in lookup_map:
            definition = lookup_map[str(mutant)]
        else:
            definition = lookup_map["empty"]

        new_row = row + definition
        results_writer.writerow(new_row)
        linecount += 1
