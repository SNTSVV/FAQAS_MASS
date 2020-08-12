#!/usr/bin/env python3.7
from scipy import spatial
import numpy as np

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

a_arr = np.array(a)
b_arr = np.array(b)

cosine = 1 - spatial.distance.cosine(a_arr, b_arr)

print(cosine)
