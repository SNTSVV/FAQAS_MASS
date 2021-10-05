#
# Copyright (c) University of Luxembourg 2020.
# Created by Fabrizio PASTORE, fabrizio.pastore@uni.lu, SnT, 2020.
# Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
# Modified by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

import sys
import os
import csv

elements = -1
positions = {}
operators = {}
operations = {}
faultModelsDef = ""
operatorsCount = 0
lastFM = ""
lastItem = -1
sizeDef = ""
lastSpan = ""
fmID = 0

SINGLETON_FM = os.getenv("_FAQAS_SINGLETON_FM", 'False').lower() in ['true', '1']
INITIAL_PADDING = os.getenv("_FAQAS_INITIAL_PADDING", '0').lower()


def newBF(item, _span, _type, _min, _max, _state, _value):
    global operations
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=BF;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].min="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].max="+_max+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].state="+_state+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].value="+_value+";\n"

    operations[elements] = 0


def newHV(item, _span, _type, _value):
    global operations
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=HV;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].value="+_value+";\n"

    operations[elements] = 0


def newVOR(item, _span, _type, _min, _max, _delta):
    global operators
    global operations
    global elements
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=VOR;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].min="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].max="+_max+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0
    currentOperator = operators[elements]
    currentItem = positions[elements]

    elements += 1
    operations[elements] = 1
    positions[elements] = currentItem
    operators[elements] = currentOperator


def newFVOR(item, _span, _type, _min, _max):
    global operators
    global operations
    global elements
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=FVOR;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].min="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].max="+_max+";\n"

    operations[elements] = 0


def newVAT(item, _span, _type, _threshold, _delta):
    global operators
    global operations
    global elements
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=VAT;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].threshold="+_threshold+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0


def newFVAT(item, _span, _type, _threshold, _delta):
    global operators
    global operations
    global elements
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=FVAT;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].threshold="+_threshold+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0


def newVBT(item, _span, _type, _threshold, _delta):
    global operators
    global operations
    global elements
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=VBT;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].threshold="+_threshold+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0


def newFVBT(item, _span, _type, _threshold, _delta):
    global operators
    global operations
    global elements
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=FVBT;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].threshold="+_threshold+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0


def newIV(item, _span, _type, _value):
    global operators
    global operations
    global elements
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=IV;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].value="+_value+";\n"

    operations[elements] = 0


def newINV(item, _span, _type, _min, _max, _delta, _value):
    global operators
    global operations
    global elements
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=INV;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].min="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].max="+_max+";\n"
    # faultModelsDef += "fm->items["+str(item)+"].operators[" + \
    #     str(operatorsCount)+"].delta="+_delta+";\n"
    # faultModelsDef += "fm->items["+str(item)+"].operators[" + \
    #     str(operatorsCount)+"].value="+_value+";\n"

    operations[elements] = 0


def newSS(item, _span, _type, _threshold, _delta):
    global operators
    global operations
    global elements
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=SS;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0


def newASA(item, _span, _type, _threshold, _delta, _value):
    global operators
    global operations
    global elements
    global faultModelsDef
    global operatorsCount

    faultModelsDef += "\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=ASA;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].threshold="+_threshold+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].delta="+_delta+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators[" + \
        str(operatorsCount)+"].value="+_value+";\n"

    operations[elements] = 0


def mutateFunctionDef(fm):

    global faultModelsDef

    faultModelsDef += "\n\n#ifdef __cplusplus\n\n"

    faultModelsDef += "void mutate_FM_" + fm + "(std::vector<" \
        + str(sys.argv[1]) + "> *v){\n"
    faultModelsDef += "    FaultModel *fm = _FAQAS_" + fm + "_FM();\n"
    faultModelsDef += "    _FAQAS_mutate(v->data(),fm);\n"
    faultModelsDef += "    _FAQAS_delete_FM(fm);\n}\n"

    faultModelsDef += "\n#else\n\n"

    faultModelsDef += "void mutate_FM_" + fm + "( "+str(sys.argv[1])+" *v){\n"
    faultModelsDef += "    FaultModel *fm = _FAQAS_" + fm + "_FM();\n"
    faultModelsDef += "    _FAQAS_mutate(v,fm);\n"
    faultModelsDef += "    _FAQAS_delete_FM(fm);\n}\n"

    faultModelsDef += "\n#endif\n\n"


def closeFaultModelsDef(last=False):
    global lastItem
    global operatorsCount
    global faultModelsDef
    global sizeDef
    global elements

    size = int(lastItem)+1
    sizeDef += "#define SIZE_"+lastFM+" "+str(size)+"\n"

    if ( last == True ):
        maxOperation = elements+1
    else:
        maxOperation = elements

    faultModelsDef += "fm->maxOperation = "+str(maxOperation)+";\n"
    faultModelsDef += "return fm;\n"
    faultModelsDef += "}\n"


def closeOperators():
    global lastItem
    global lastType
    global operatorsCount
    global faultModelsDef
    global lastSpan
    faultModelsDef += "fm->items["+str(lastItem)+"].operatorsN="+str(operatorsCount+1)+";\n"
    faultModelsDef += "fm->items["+str(lastItem)+"].span="+str(lastSpan)+";\n"
    faultModelsDef += "fm->items["+str(lastItem)+"].type="+str(lastType)+";\n"


def processRow(row):
    print(row)
    global lastFM
    global lastItem
    global lastType
    global lastSpan
    global positions
    global operators
    global operations
    global operatorsCount
    global elements
    global faultModelsDef
    global fmID
    global SINGLETON_FM

    _p = 0
    FM = row[_p]

    _p = _p+1
    item = row[_p]

    _p = _p+1
    _span = row[_p]

    _p = _p+1
    _type = row[_p]

    _p = _p+1
    FT = row[_p]

    _p = _p+1
    _min = row[_p]

    _p = _p+1
    _max = row[_p]

    _p = _p+1
    _threshold = row[_p]

    _p = _p+1
    _delta = row[_p]

    _p = _p+1
    _state = row[_p]

    _p = _p+1
    _value = row[_p]

    elements += 1
    positions[elements] = item

    if FM == lastFM:
        if lastItem == item:
            operatorsCount += 1
        else:
            closeOperators()
            operatorsCount = 0
    else:
        if lastFM != "":
            closeOperators()
            closeFaultModelsDef()
            mutateFunctionDef(lastFM)

        lastItem = -1

        if SINGLETON_FM == True:
            faultModelsDef += "struct FaultModel* _FAQAS_"+FM+"_FM_ptr = 0;\n"
            faultModelsDef += "\nvoid _FAQAS_delete_"+FM+"_FM(void){\n"
            faultModelsDef += "__FAQAS_delete_FM(_FAQAS_"+FM+"_FM_ptr);\n"
            faultModelsDef += "_FAQAS_"+FM+"_FM_ptr = 0;\n"
            faultModelsDef += "}\n"

        faultModelsDef += "struct FaultModel* _FAQAS_"+FM+"_FM(){\n"
        if SINGLETON_FM == True:
            faultModelsDef += "if ( _FAQAS_"+FM+"_FM_ptr != 0 ){ return _FAQAS_"+FM+"_FM_ptr;}\n"
        else:
            faultModelsDef += "FaultModel *fm = _FAQAS_create_FM(SIZE_"+FM+");\n"

        if SINGLETON_FM == True:
            faultModelsDef += "atexit(_FAQAS_delete_"+FM+"_FM);\n"
            faultModelsDef += "FaultModel *fm = _FAQAS_create_FM(SIZE_"+FM+");\n"
            faultModelsDef += "_FAQAS_"+FM+"_FM_ptr = fm;\n"

        faultModelsDef += "fm->ID = " + str(fmID) + ";\n"
        faultModelsDef += "fm->minOperation = "+str(elements)+";\n"

        fmID += 1

    operators[elements] = operatorsCount
    if _type == 'HEX':
        _type = 'INT'
    if FT == 'BF':
        newBF(item, _span, _type, _min, _max, _state, _value)
    if FT == 'HV':
        newHV(item, _span, _type, _value)
    if FT == 'VOR':
        newVOR(item, _span, _type, _min, _max, _delta)
    if FT == 'FVOR':
        newFVOR(item, _span, _type, _min, _max)
    if FT == 'VAT':
        newVAT(item, _span, _type, _threshold, _delta)
    if FT == 'FVAT':
        newFVAT(item, _span, _type, _threshold, _delta)
    if FT == 'VBT':
        newVBT(item, _span, _type, _threshold, _delta)
    if FT == 'FVBT':
        newFVBT(item, _span, _type, _threshold, _delta)
    if FT == 'IV':
        newIV(item, _span, _type, _value)
    if FT == 'INV':
        newINV(item, _span, _type, _min, _max, _delta, _value)
    if FT == 'SS':
        newSS(item, _span, _type, _threshold, _delta)
    if FT == 'ASA':
        newASA(item, _span, _type, _threshold, _delta, _value)

    lastFM = FM
    lastItem = item
    lastSpan = _span
    lastType = _type


def generateSelectFunctionContent(array):
    selectItem = ""
    for k in array:
        selectItem += "if ( MUTATION == "+str(k)+" )\n"
        selectItem += "    return "+str(array[k])+";\n"
    return selectItem


if __name__ == "__main__":
    argv = len(sys.argv)
    if (argv != 3):
        print("Usage: generateDataMutator.py <BufferType> <FaultModel.csv>")

bufferType = sys.argv[1]

print(str(bufferType))

fileName = sys.argv[2]

# this lines produce an output csv files with the numerical ID of every mutant

with open(fileName, 'r') as fault_model, open("FAQAS_mutants_table.csv", 'w') as map:
    map.truncate(0)
    fault_model_reader = csv.reader(fault_model, delimiter=',')
    map_writer = csv.writer(map, delimiter=',')
    mutant_count = -1
    duplicate = 0
    for row in fault_model_reader:
        new_row = row
        if mutant_count == -1:
            column_0 = 'MutationOpt'
        else:
            column_0 = mutant_count
        new_row.insert(0, column_0)
        map_writer.writerow(new_row)
        mutant_count += 1

        if row[5] == 'VOR':  # at the moment VOR is the only one to double
            new_row[0] = mutant_count
            map_writer.writerow(new_row)
            mutant_count += 1

fault_models = set()
with open(fileName, 'r') as definitions, open("function_calls.out", 'a') as funct:
    def_reader = csv.reader(definitions, delimiter=',')
    for op_line in def_reader:
        fault_models.add(op_line[0])
    funct.write("To mutate insert probes using the following templates:\n")
    print("To mutate insert probes using the following templates, you can find them in the 'function_calls.out' file:\n")
    for fmx in fault_models:
        funct.write("void mutate_FM_" + fmx + "(std::vector<" + str(sys.argv[1]) + "> *v);\n")
        print("mutate_FM_" + fmx + "(std::vector<" + str(sys.argv[1]) + "> *v);\n")


with open(fileName) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
        if line_count == 0:
            print('Column names are {", ".join(row)}')
            line_count += 1
        else:
            processRow(row)
            line_count += 1

closeOperators()
closeFaultModelsDef(True)
mutateFunctionDef(lastFM)

maxFMO = "//max MUTATIONOPT="+str(elements)

selectItem = "int _FAQAS_selectItem(){\n"
selectItem += generateSelectFunctionContent(positions)
selectItem += "return -999;\n"
selectItem += "}\n"
selectItem += "int _FAQAS_INITIAL_PADDING =" + str(INITIAL_PADDING)  + "; \n"

selectOperator = "int _FAQAS_selectOperator(){\n"
selectOperator += generateSelectFunctionContent(operators)
selectOperator += "return -999;\n"
selectOperator += "}\n"

selectOperations = "int _FAQAS_selectOperation(){\n"
selectOperations += generateSelectFunctionContent(operations)
selectOperations += "return -999;\n"
selectOperations += "}\n"

outfile = open("FAQAS_dataDrivenMutator.h", "wt")
outfile.write(maxFMO)


with open('DDB_TEMPLATE_header.c', 'r') as tfile:
    data = tfile.read().replace('BUFFER_TYPE', str(bufferType))
    outfile.write(data)
    tfile.close()

sizeDef += "\n\n"
outfile.write(sizeDef)

faultModelsDef += "\n\n"
outfile.write(faultModelsDef)

outfile.write(selectItem)
outfile.write(selectOperator)
outfile.write(selectOperations)

outfile.write("\n\n#define APPLY_ONE_MUTATION 0\n\n")


outfile.write("int FAQAS_fmCov;\n")
outfile.write("void _FAQAS_fmCoverage(int fm){\n")
outfile.write("    switch (fm){\n")
for x in range(fmID):
    line = "    case {}:\n"
    outfile.write(line.format(x))
    outfile.write("    FAQAS_fmCov++;\n")
    outfile.write("    break;\n")


outfile.write("    default:\n")
outfile.write("    break;\n")
outfile.write("    }\n")
outfile.write("}\n")
outfile.write("//END _FAQAS_fmCoverage\n")

with open('DDB_TEMPLATE_footer.c', 'r') as tfile:
    data = tfile.read().replace('BUFFER_TYPE', str(bufferType))
    outfile.write(data)
    tfile.close()

outfile.close()
