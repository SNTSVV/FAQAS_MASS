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
            print(key + " is equivalent")
#        else:
#            print(key + " is not equivalent")

hashesFile = str(sys.argv[1])
hashesFileDir = os.path.dirname(hashesFile)
originalHashFile = str(sys.argv[2])
readHashes(hashesFile, originalHashFile)

filterEquivalents()

