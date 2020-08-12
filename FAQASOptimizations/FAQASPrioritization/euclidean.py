#!/usr/bin/env python3.7

import numpy

testATemp = open('testATemp')
testALines = testATemp.readlines()

a = []

for line in testALines:
	a.append(line.strip())

a = list(map(int, a))

testBTemp = open('testBTemp')
testBLines = testBTemp.readlines()

b = []

for line in testBLines:
	b.append(line.strip())

b = list(map(int, b))

a_arr = numpy.array(a)
b_arr = numpy.array(b)

dist = numpy.linalg.norm(a_arr - b_arr)

print(dist)

