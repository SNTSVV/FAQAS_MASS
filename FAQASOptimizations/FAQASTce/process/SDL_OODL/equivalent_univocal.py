import os, sys

maxConf = int(sys.argv[1])

def getAllMutants():
	allFiles = open('all', 'r')
	
	allMutants = []
	
	for line in allFiles:
		allMutants.append(line.strip())

	return allMutants		


def checkIfRedundant(mutant, conf):
	with open('redundants_' + str(conf)) as f:
		if mutant in f.read():
			return True

def find_univocal_redundants():
	univocalFile = open('univocal_redundants.csv', 'w')
	univocalList = open('univocal_redundants_list', 'w')

	mutants = getAllMutants()

	for mutant in mutants:
		univocalFile.write(mutant + ';')
		
		univocalConf = -1
		univocalSum = 0 
	
		for i in range(0, maxConf + 1):
			if checkIfRedundant(mutant, i):
				univocalFile.write('1')
				univocalSum += 1
				univocalConf = i
			else:
				univocalFile.write('0')

			if i < maxConf:
				univocalFile.write(';')				
		
		if univocalSum == 1:
			print("mutant " + mutant + " is univocal redundant, conf " + str(univocalConf))
			univocalList.write(mutant + ";" + str(univocalConf) + "\n")			
		univocalFile.write('\n')	
	return

def checkIfEquivalent(mutant, conf):
	with open('equivalents_' + str(conf)) as f:
		if mutant in f.read():
			return True

def find_univocal_equivalents():
	univocalFile = open('univocal_equivalents.csv', 'w')
	univocalList = open('univocal_equivalents_list', 'w')
	
	mutants = getAllMutants()

	for mutant in mutants:
		univocalFile.write(mutant + ';')
		
		univocalConf = -1
		univocalSum = 0

		for i in range(0, maxConf + 1):
			if checkIfEquivalent(mutant, i):
				univocalFile.write('1')
				univocalSum += 1
				univocalConf = i
			else:
				univocalFile.write('0')
			if i < maxConf:
				univocalFile.write(';')				

		if univocalSum == 1:
			print("mutant " + mutant + " is univocal equivalent, conf " + str(univocalConf))
			univocalList.write(mutant + ";" + str(univocalConf) + "\n")
		univocalFile.write('\n')	
	return


find_univocal_equivalents()
find_univocal_redundants()

