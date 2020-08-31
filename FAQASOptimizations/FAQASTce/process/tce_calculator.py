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
        hashesDict[full_line[0]] = full_line[1]

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
#        print('-------------------------')
#        print('mutant ' + key + ' ' + value)
        key_splitted = key.split('.')
        filename = key_splitted[0]
        function = key_splitted[5]
    
        matchingFileAndFunction = [uniqueKey for uniqueKey, uniqueValue in unique_mutants.items() if filename in uniqueKey and function in uniqueKey]
        redundant = 0
        for mutant in matchingFileAndFunction:
#            print("     comparing with " + mutant + ' '+ unique_mutants[mutant]) 
            if value == unique_mutants[mutant]:
                redundant = 1
 #               print("         REDUNDANT with " + mutant + ' ' + unique_mutants[mutant]) 
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

hashesFile = str(sys.argv[1])
hashesFileDir = os.path.dirname(hashesFile)
originalHashFile = str(sys.argv[2])
readHashes(hashesFile, originalHashFile)

print("Original size: " + str(len(hashesDict)))

filterEquivalents()

print("Equivalents: " + str(len(equivalents)))

filterRedundants()

print("Redundants: " + str(len(redundants)))
print("Uniques: " + str(len(unique_mutants)))


