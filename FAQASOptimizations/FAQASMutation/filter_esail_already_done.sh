#!/bin/bash

all=/mnt/irisgpfs/projects/faqas/srcirorfaqas/FAQASOptimizations/FAQASTce/process/ESAIL/all_filtered
SRC_MUTANTS=/home/users/ocornejo/ESAIL/noneqred/src-mutants

while IFS="" read -r p || [ -n "$p" ];do
    read name <<< $(awk -F'|' '{print $1}' <<< "$p")
    read location <<< $(awk -F'|' '{print $2}' <<< "$p")
    
    find $SRC_MUTANTS/$location -name "${name}.c" -delete 

done < $all | wc -l

