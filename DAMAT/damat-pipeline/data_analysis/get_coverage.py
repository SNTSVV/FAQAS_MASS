
#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

import sys
import re
from string import Template

working_folder = sys.argv[3]

nameInFile = str(working_folder) + "/coverage_" + str(sys.argv[1]) + ".csv"
nameOutFile = str(working_folder) + "/readable_coverage_" + str(sys.argv[1]) + ".csv"
mutator = sys.argv[2]

outputFile = open(nameOutFile, "a+")
outputFile.truncate(0)

table = []

# open the .h and get the names
with open(str(mutator), "rt") as mutator:
    data = mutator.readlines()
for line in data:
    if line.__contains__('#define SIZE_'):
        definition = re.split('#define SIZE_', line)
        name = definition[1].split()
        table.append(name[0].strip())

mutator.close()

# outputFile.write('Test,FaultModel,Status,Times\n')
print('Test,FaultModel,status,times\n')
lineTemplate = Template('$test,$FM,$status,$times\n')

with open(str(nameInFile), "rt") as coverage:
    data = coverage.readlines()
for i in range(len(table)):
    status = 'not_covered'
    times = 0
    covData = {"test": str(sys.argv[1]), "FM": table[i], "status": status, "times": times}
    for line in data:
        labrador = line.split()
        if (int(labrador[1].strip()) == i):
            covData["status"] = 'covered'
            covData["times"] = covData["times"] + 1

    newLine = lineTemplate.substitute(**covData)
    outputFile.write(newLine)
    print(newLine)

coverage.close()
outputFile.close()
