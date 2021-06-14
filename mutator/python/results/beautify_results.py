import csv
import sys


def cancelUnusedParameters(FC_definition):
    # substitute unused parameters w/ - to improve readability
    # on FAQAS_mutants_table 5 FC, 6 Min, 7 Max, 8 T, 9 D, 10 S, 11 V
    unused_parameters = {
      "VAT": [6, 7, 10, 11],
      "VBT": [6, 7, 10, 11],
      "VOR": [8, 10, 11],
      "BF": [8, 9],
      "INV": [8, 10],
      "IV": [6, 7, 8, 9, 10],
      "ASA": [6, 7, 10],
      "SS": [6, 7, 8, 10, 11],
      "HV": [6, 7, 8, 9, 10]
    }
    faultClass = FC_definition[5]

    if faultClass in unused_parameters:

        for parameter in unused_parameters[str(faultClass)]:
            FC_definition[parameter] = "u"

    return FC_definition


if sys.argv[1] == "h":
    raw_has_header = True

lookup_map = {}

with open("FAQAS_mutants_table.csv", 'r') as map:
    linecount = 0
    map_reader = csv.reader(map, delimiter=',')
    for definition in map_reader:
        cancelUnusedParameters(definition)
        mutant = definition.pop(0)
        if linecount == 0:
            mutant = "header"
        lookup_map[str(mutant)] = definition
        linecount += 1

with open("main.csv", 'r') as raw, open("results.csv", 'w') as results:
    results.truncate(0)
    raw_reader = csv.reader(raw, delimiter=',')
    results_writer = csv.writer(results, delimiter=',')

    linecount = 0

    for row in raw_reader:
        mutant = row[0]
        if raw_has_header and linecount == 0:
            mutant = "header"
        new_row = row
        if mutant in lookup_map:
            definition = lookup_map[str(mutant)]
            new_row = row + definition
        results_writer.writerow(new_row)
        linecount += 1
