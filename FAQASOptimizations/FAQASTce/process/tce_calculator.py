import sys, os

unique_mutants = {}
equivalents = {}
redundants = {}

hashesDict = {}

originalHash = ""


def readHashes(filePath, originalFilePath):
    hashesFile = open(filePath, 'r')
    
    for line in hashesFile:
        full_line = line.strip().split(';')
        hashesDict[full_line[0]+ '|' + full_line[1]] = full_line[2]

    global originalHash

    f = open(originalFilePath, 'r')
    originalHash = f.readline().strip()

def filterEquivalents():
    for key, value in hashesDict.items():
        if value == originalHash:
            equivalents[key] = value

    for key, value in equivalents.items():
        del hashesDict[key]


def filterRedundants():
    count = 0
    for key, value in hashesDict.items():
        print('-------------------------')
        print('mutant ' + key + ' ' + value)
        key_splitted_w_location = key.split('|')
        location = key_splitted_w_location[1]
        key_splitted = key_splitted_w_location[0].split('.')
        filename = key_splitted[0]
        function = key_splitted[5]
    
        matchingFileAndFunction = [uniqueKey for uniqueKey, uniqueValue in unique_mutants.items() if filename in uniqueKey and function in uniqueKey and location in uniqueKey]
        redundant = 0
        for mutant in matchingFileAndFunction:
            print("     comparing with " + mutant + ' '+ unique_mutants[mutant]) 
            if value == unique_mutants[mutant]:
                redundant = 1
                print("         REDUNDANT with " + mutant + ' ' + unique_mutants[mutant]) 
                break
 
        if redundant == 0:
            unique_mutants[key] = value
        else:
            redundants[key] = value 


        if count == 100000000:
            break
        else:
            count += 1
    return

def printAllMutants():
	fileAllMutants = open('all', 'w')
	for key, value in hashesDict.items():
		fileAllMutants.write(key + '\n')

def printEquivalents():
	fileEquivalents = open('equivalents', 'w')
	
	for key, value in equivalents.items():	
		fileEquivalents.write(key + '\n')

def printRedundants():
	fileRedundants = open('redundants', 'w')
	
	for key, value in redundants.items():	
		fileRedundants.write(key + '\n')

hashesFile = str(sys.argv[1])
hashesFileDir = os.path.dirname(hashesFile)
originalHashFile = str(sys.argv[2])
readHashes(hashesFile, originalHashFile)

printAllMutants()

print("T: " + str(len(hashesDict)))

filterEquivalents()

print("E: " + str(len(equivalents)))

filterRedundants()

print("R: " + str(len(redundants)))
#print("Uniques: " + str(len(unique_mutants)))

printEquivalents()
printRedundants()

