#!/bin/bash
FOLDER=$1
file=$2

echo ABS `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.ABS\." | wc -l`
echo AOR `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.AOR\." | wc -l`
echo ICR `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.ICR\." | wc -l`
echo LCR `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.LCR\." | wc -l`
echo ROR `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.ROR\." | wc -l`
echo SDL `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.SDL\." | wc -l`
echo UOI `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.UOI\." | wc -l`
echo AOD `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.AOD\." | wc -l`
echo LOD `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.LOD\." | wc -l`
echo ROD `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.ROD\." | wc -l`
echo BOD `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.BOD\." | wc -l`
echo SOD `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.SOD\." | wc -l`
echo LVR `find $FOLDER -maxdepth 2 -name "$file" | xargs cat | sort | uniq | grep "\.LVR\." | wc -l`
