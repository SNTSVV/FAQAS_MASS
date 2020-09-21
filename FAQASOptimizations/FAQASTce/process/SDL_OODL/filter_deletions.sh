#!/bin/bash

WORKING=/home/users/ocornejo/MLFS/TCE
RUNS=$WORKING/runs_sdl

for hash_file in $(find $RUNS -name 'hashes.csv');do
    echo $hash_file
    awk '!/ABS|AOR|ICR|LCR|ROR|UOI|AOD|LOD|ROD|BOD|SOD|LVR/' $hash_file > temp
    mv temp $hash_file
done

RUNS=$WORKING/runs_sdl_oodl

for hash_file in $(find $RUNS -name 'hashes.csv');do
    echo $hash_file
    awk '!/ABS|AOR|ICR|LCR|ROR|UOI|LVR/' $hash_file > temp
    mv temp $hash_file
done
