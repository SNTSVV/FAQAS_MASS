#!/bin/bash
file=$1

echo ABS `find . -maxdepth 2 -name "$file" | xargs cat | grep ABS | wc -l`
echo AOR `find . -maxdepth 2 -name "$file" | xargs cat | grep AOR | wc -l`
echo ICR `find . -maxdepth 2 -name "$file" | xargs cat | grep ICR | wc -l`
echo LCR `find . -maxdepth 2 -name "$file" | xargs cat | grep LCR | wc -l`
echo ROR `find . -maxdepth 2 -name "$file" | xargs cat | grep ROR | wc -l`
echo SDL `find . -maxdepth 2 -name "$file" | xargs cat | grep SDL | wc -l`
echo UOI `find . -maxdepth 2 -name "$file" | xargs cat | grep UOI | wc -l`
echo AOD `find . -maxdepth 2 -name "$file" | xargs cat | grep AOD | wc -l`
echo LOD `find . -maxdepth 2 -name "$file" | xargs cat | grep LOD | wc -l`
echo ROD `find . -maxdepth 2 -name "$file" | xargs cat | grep ROD | wc -l`
echo BOD `find . -maxdepth 2 -name "$file" | xargs cat | grep BOD | wc -l`
echo SOD `find . -maxdepth 2 -name "$file" | xargs cat | grep SOD | wc -l`
echo LVR `find . -maxdepth 2 -name "$file" | xargs cat | grep LVR | wc -l`
