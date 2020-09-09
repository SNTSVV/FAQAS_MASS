#!/bin/bash
file=$1
opt=$2

echo ABS `find . -name "$file" | xargs cat | grep ";${opt}" | grep ABS | wc -l`
echo AOR `find . -name "$file" | xargs cat | grep ";${opt}" | grep AOR | wc -l`
echo ICR `find . -name "$file" | xargs cat | grep ";${opt}" | grep ICR | wc -l`
echo LCR `find . -name "$file" | xargs cat | grep ";${opt}" | grep LCR | wc -l`
echo ROR `find . -name "$file" | xargs cat | grep ";${opt}" | grep ROR | wc -l`
echo SDL `find . -name "$file" | xargs cat | grep ";${opt}" | grep SDL | wc -l`
echo UOI `find . -name "$file" | xargs cat | grep ";${opt}" | grep UOI | wc -l`
echo AOD `find . -name "$file" | xargs cat | grep ";${opt}" | grep AOD | wc -l`
echo LOD `find . -name "$file" | xargs cat | grep ";${opt}" | grep LOD | wc -l`
echo ROD `find . -name "$file" | xargs cat | grep ";${opt}" | grep ROD | wc -l`
echo BOD `find . -name "$file" | xargs cat | grep ";${opt}" | grep BOD | wc -l`
echo SOD `find . -name "$file" | xargs cat | grep ";${opt}" | grep SOD | wc -l`
echo LVR `find . -name "$file" | xargs cat | grep ";${opt}" | grep LVR | wc -l`

