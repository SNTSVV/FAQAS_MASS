
#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

import sys
import csv
import os.path

# os.path.isfile(path)
working_folder = sys.argv[2]

test_case = str(sys.argv[1])

nameInFile = str(working_folder) + "/coverage_" + test_case + ".csv"
nameOutFile = str(working_folder) + "/readable_coverage_" + test_case + ".csv"

outputFile = open(nameOutFile, "a+")

Calls = 0
Mutations = 0

if os.path.isfile(nameInFile):
    with open(str(nameInFile), "rt") as coverage:
        Operator = -111
        raw_data = csv.reader(coverage, delimiter=',')
        header = 0
        for line in raw_data:
            if header == 0:
                Operator = int(line[0])
                header = 1
            Calls = int(line[1])
            Mutations = int(Mutations) + int(line[2])

    # outputFile.write('Operator,TestCase,Calls,Mutations\n')
    outputFile.write(str(Operator) + "," + test_case + "," + str(Calls) + "," + str(Mutations) + '\n')

outputFile.close()
