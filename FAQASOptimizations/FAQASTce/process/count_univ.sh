#!/bin/bash
file=$1
opt=$2

echo ABS `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep ABS | wc -l`
echo AOR `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep AOR | wc -l`
echo ICR `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep ICR | wc -l`
echo LCR `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep LCR | wc -l`
echo ROR `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep ROR | wc -l`
echo SDL `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep SDL | wc -l`
echo UOI `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep UOI | wc -l`
echo AOD `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep AOD | wc -l`
echo LOD `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep LOD | wc -l`
echo ROD `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep ROD | wc -l`
echo BOD `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep BOD | wc -l`
echo SOD `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep SOD | wc -l`
echo LVR `find . -maxdepth 2 -name "$file" | xargs cat | grep ";${opt}" | grep LVR | wc -l`

