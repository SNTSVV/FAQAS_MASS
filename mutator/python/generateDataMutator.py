import sys

import csv

elements=-1
positions={}
operators={}
operations={}
faultModelsDef=""
operatorsCount=0
lastFM=""
lastItem=""
sizeDef=""

def newBF(item,_type,_min,_max,_state):
    global operations
    global faultModelsDef
    faultModelsDef+="\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=BF;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].min="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].max="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].state="+_state+";\n"

    operations[elements] = 0
    

def newVOR(item,_type,_min,_max,_delta):
    global operators
    global operations
    global elements
    global faultModelsDef
    
    faultModelsDef+="\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].type=VOR;\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].min="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].max="+_min+";\n"
    faultModelsDef += "fm->items["+str(item)+"].operators["+str(operatorsCount)+"].delta="+_delta+";\n"

    operations[elements] = 0
    currentOperator = operators[elements]
    currentItem = positions[elements]

    elements += 1
    operations[elements] = 1
    positions[elements] = currentItem
    operators[elements] = currentOperator


def closeFaultModelsDef():
    global lastItem
    global operatorsCount
    global faultModelsDef
    global sizeDef
 
    sizeDef += "#define SIZE_"+lastFM+" "+lastItem+"\n"

    faultModelsDef+="return rm;\n"
    faultModelsDef+="}\n"


def closeOperators():
    global lastItem
    global operatorsCount
    global faultModelsDef
    faultModelsDef += "fm->items["+str(lastItem)+"].operatorsN="+str(operatorsCount+1)+";\n"

def processRow(row):
    print(row)
    global lastFM
    global lastItem
    global positions
    global operators
    global operations
    global operatorsCount
    global elements
    global faultModelsDef

    FM = row[0]

    item = row[1]
    _type = row[2]
    FT = row[3]
    _min=row[4]
    _max=row[5]
    _threshold=row[6]
    _delta=row[7]
    _state=row[8]
    
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
            closeFaultModelsDef()
            
        lastItem=""
        faultModelsDef+="struct FaultModel* _FAQAS_"+FM+"_FM(){\n"            
        faultModelsDef+="FaultModel *fm = _FAQAS_create_FM(SIZE_"+FM+");\n"

    operators[elements]=operatorsCount

    if FT == 'BF':
            newBF(item,_type,_min,_max,_state)
    if FT == 'VOR':
            newVOR(item,_type,_min,_max,_state)
    
    lastFM=FM
    lastItem=item 

 

if __name__ == "__main__":
	argv = len(sys.argv)
	if ( argv != 2 ):
		printf("Usage: generateDataMutator.py <FaultModel.csv>")

fileName=sys.argv[1]

with open(fileName) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
        if line_count == 0:
            print(f'Column names are {", ".join(row)}')
            line_count += 1
        else:
            processRow(row)
            line_count += 1

closeOperators()
closeFaultModelsDef()

print(positions)
print(operators)
print(operations)

selectItem=""

selectItem+="int selectItem(FaultModel *dm){\n"
for k in positions:
    selectItem+="if ( MUTATION == "+str(k)+" )\n"
    selectItem+="    return "+str(positions[k])+";\n"
selectItem+="}\n"

print(selectItem)

print(sizeDef)
print(faultModelsDef)
