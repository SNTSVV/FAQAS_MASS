#!/bin/bash

PRIORITIZED_DIR=$APP_RUN_DIR/PRIORITIZED
mkdir -p $PRIORITIZED_DIR

PRIORITIZED=$PRIORITIZED_DIR/prioritized.txt
touch $PRIORITIZED

REDUCED=$PRIORITIZED_DIR/reduced.txt
touch $REDUCED

if [[ -s $REDUCED ]];then
    echo "reduced test suite already generated"
    return 0
fi

for src in $(find $PROJ_SRC -name '*.c');do
    lines=$(wc -l $src | awk '{print $1}')
    count=1

    echo $src $lines
    for line in $(seq $count $lines);do
        echo prioritize.sh $COV_FILES $MUTANTS_DIR $src $line $PRIORITIZED
        source $MASS/FAQAS-CompileAndExecuteMutants/FAQAS-GeneratePrioritizedTestSuite/prioritize.sh $COV_FILES $MUTANTS_DIR $src $line $PRIORITIZED $REDUCED
    done
done
