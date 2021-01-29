#!/bin/bash

SRC_MUTANTS=$1

n=1
max_it=$2
mut_per_folder=$3

while [ "$n" -le "$max_it" ]; do
	echo $n
	
	cd $SRC_MUTANTS/..

	folder="m$n"
	mkdir $folder

	cd $SRC_MUTANTS

	for file in `find . -name '*.c' | shuf -n $mut_per_folder`; do
        cp "$file" --parents ../$folder && rm "$file";
    done
    
	cd ../	

	n=$((n+1))
done

mkdir filtered/
find . -maxdepth 1 -type d -name 'm*' -exec mv '{}' filtered/ \; 
#rm -rf $SRC_MUTANTS
#mv filtered src-mutants

#tar czf mutants.tar.gz src-mutants/
