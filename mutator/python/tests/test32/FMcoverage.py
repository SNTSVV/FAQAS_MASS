import sys
import re
from string import Template

nameOutFile = "FMCoverageReport_" + str(sys.argv[1]) + ".csv"
outputFile = open(nameOutFile, "a+")
outputFile.truncate(0)

hloutputFile = open("hlreport.csv", "a+")
hloutputFile.truncate(0)

table = []

with open("FAQAS_dataDrivenMutator.h.gcov", "rt") as mutator:
    data = mutator.readlines()
for line in data:
    if line.__contains__('void _FAQAS_fmCoverage'):
        start = data.index(line)
    if line.__contains__('END _FAQAS_fmCoverage'):
        end = data.index(line)
    if line.__contains__('#define SIZE_'):
        definition = re.split('#define SIZE_', line)
        name = definition[1].split()
        table.append(name[0].strip())

outputFile.write('Test,FaultModel,Status,Times\n')
print('Test,FaultModel,status,times\n')
lineTemplate = Template('$test,$FM,$status,$times\n')
for i in range(start, end):
    if data[i].__contains__('case'):
        case = re.split(':', data[i])
        cuccioliDiCane = case[2].strip()
        FaultModelNr = cuccioliDiCane.split()[1]
        FaultModelName = table[int(FaultModelNr)]
        secondLine = re.split(':', data[i+1])
        status = 'covered'
        if secondLine[0].strip() == '#####':
            secondLine[0] = '0'
            status = 'not_covered'
        covData = {"test": str(sys.argv[1]), "FM": FaultModelName, "status": status, "times": secondLine[0].strip()}
        newLine = lineTemplate.substitute(**covData)
        outputFile.write(newLine)
        hloutputFile.write(newLine)
        print(newLine)


mutator.close()
outputFile.close()
hloutputFile.close()
