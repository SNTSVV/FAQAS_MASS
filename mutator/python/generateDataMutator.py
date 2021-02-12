#
# Copyright (c) University of Luxembourg 2020.
# Created by Fabrizio PASTORE, fabrizio.pastore@uni.lu, SnT, 2020.
# Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
#

import sys

import csv

elements=-1
positions={}
operators={}
operations={}
faultModelsDef=""
operatorsCount=0
lastFM=""
lastItem=-1
sizeDef=""
lastSpan=""
fmID=0

def newBF(item,_span,_type,_min,_max,_state,_value):
    global operations
    global faultModelsDef
    faultModelsDef+="\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=BF;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].min="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].max="+_max+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].state="+_state+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].value="+_value+";\n"

    operations[elements] = 0


def newVOR(item,_span,_type,_min,_max,_delta):
    global operators
    global operations
    global elements
    global faultModelsDef

    faultModelsDef+="\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=VOR;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].min="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].max="+_max+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0
    currentOperator = operators[elements]
    currentItem = positions[elements]

    elements += 1
    operations[elements] = 1
    positions[elements] = currentItem
    operators[elements] = currentOperator


def newVAT(item,_span,_type,_threshold,_delta):
    global operators
    global operations
    global elements
    global faultModelsDef

    faultModelsDef+="\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=VAT;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].threshold="+_threshold+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0


def newVBT(item,_span,_type,_threshold,_delta):
    global operators
    global operations
    global elements
    global faultModelsDef

    faultModelsDef+="\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=VBT;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].threshold="+_threshold+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0


def newIV(item,_span,_type,_value):
    global operators
    global operations
    global elements
    global faultModelsDef

    faultModelsDef+="\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=IV;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].value="+_value+";\n"

    operations[elements] = 0


def newINV(item,_span,_type,_min,_max,_delta,_value):
    global operators
    global operations
    global elements
    global faultModelsDef

    faultModelsDef+="\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=INV;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].min="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].max="+_max+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].delta="+_delta+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].value="+_value+";\n"

    operations[elements] = 0


def newSS(item,_span,_type,_threshold,_delta):
    global operators
    global operations
    global elements
    global faultModelsDef

    faultModelsDef+="\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=SS;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].threshold="+_threshold+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0


def newASA(item,_span,_type,_threshold,_delta,_value):
    global operators
    global operations
    global elements
    global faultModelsDef

    faultModelsDef+="\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=ASA;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].threshold="+_threshold+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].delta="+_delta+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].value="+_value+";\n"

    operations[elements] = 0


def closeFaultModelsDef():
    global lastItem
    global operatorsCount
    global faultModelsDef
    global sizeDef

    size=int(lastItem)+1
    sizeDef += "#define SIZE_"+lastFM+" "+str(size)+"\n"

    faultModelsDef+="return fm;\n"
    faultModelsDef+="}\n"


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

    _p=0;
    FM = row[_p]

    _p=_p+1;
    item = row[_p]

    _p=_p+1;
    _span = row[_p]

    _p=_p+1;
    _type = row[_p]

    _p=_p+1;
    FT = row[_p]
    _p=_p+1;
    _min=row[_p]
    _p=_p+1;
    _max=row[_p]
    _p=_p+1;
    _threshold=row[_p]
    _p=_p+1;
    _delta=row[_p]
    _p=_p+1;
    _state=row[_p]
    _p=_p+1;
    _value=row[_p]

    elements+=1

    positions[elements]=item

    if FM == lastFM:
        if lastItem == item:
            operatorsCount+=1
        else:
            closeOperators()
            operatorsCount=0
    else:
        if lastFM != "":
            closeOperators()
            closeFaultModelsDef()

        lastItem=-1
        faultModelsDef+="struct FaultModel* _FAQAS_"+FM+"_FM(){\n"
        faultModelsDef+="FaultModel *fm = _FAQAS_create_FM(SIZE_"+FM+");\n"
        faultModelsDef += "fm->ID = " + str(fmID) + ";\n"
        fmID += 1

    operators[elements]=operatorsCount

    if FT == 'BF':
            newBF(item,_span,_type,_min,_max,_state,_value)
    if FT == 'VOR':
            newVOR(item,_span,_type,_min,_max,_delta)
    if FT == 'VAT':
            newVAT(item,_span,_type,_threshold,_delta)
    if FT == 'VBT':
            newVBT(item,_span,_type,_threshold,_delta)
    if FT == 'IV':
            newIV(item,_span,_type,_value)
    if FT == 'INV':
            newINV(item,_span,_type,_min,_max,_delta,_value)
    if FT == 'SS':
            newSS(item,_span,_type,_threshold,_delta)
    if FT == 'ASA':
            newASA(item,_span,_type,_threshold,_delta,_value)

    lastFM=FM
    lastItem=item
    lastSpan=_span
    lastType=_type


def generateSelectFunctionContent(array):
    selectItem=""
    for k in array:
        selectItem+="if ( MUTATION == "+str(k)+" )\n"
        selectItem+="    return "+str(array[k])+";\n"
    return selectItem




if __name__ == "__main__":
	argv = len(sys.argv)
	if ( argv != 3 ):
		print("Usage: generateDataMutator.py <BufferType> <FaultModel.csv>")

bufferType=sys.argv[1]
fileName=sys.argv[2]

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
closeFaultModelsDef()

maxFMO="//max MUTATIONOPT="+str(elements)

selectItem="int _FAQAS_selectItem(){\n"
selectItem+=generateSelectFunctionContent(positions)
selectItem+="return -999;\n"
selectItem+="}\n"

selectOperator="int _FAQAS_selectOperator(){\n"
selectOperator+=generateSelectFunctionContent(operators)
selectOperator+="return -999;\n"
selectOperator+="}\n"

selectOperations="int _FAQAS_selectOperation(){\n"
selectOperations+=generateSelectFunctionContent(operations)
selectOperations+="return -999;\n"
selectOperations+="}\n"

outfile = open("FAQAS_dataDrivenMutator.h", "wt")
outfile.write(maxFMO)


with open('DDB_TEMPLATE_header.c', 'r') as tfile:
    data = tfile.read().replace('BUFFER_TYPE', bufferType )
    outfile.write(data)
    tfile.close()

sizeDef+="\n\n"
outfile.write(sizeDef)

faultModelsDef+="\n\n"
outfile.write(faultModelsDef)

outfile.write(selectItem)
outfile.write(selectOperator)
outfile.write(selectOperations)

#TODO: we need to add an option to specify the number of mutations to apply
outfile.write("\n\n#define APPLY_ONE_MUTATION 0\n\n")

with open('DDB_TEMPLATE_footer.c', 'r') as tfile:
    data = tfile.read().replace('BUFFER_TYPE', bufferType )
    outfile.write(data)
    tfile.close()

outfile.close()
