#!/bin/bash

FLAG=$1
run=$2

export PROJ_ORIGINAL_BUILD=$PROJ/original_build/$COMPILED

# backing up original makefile
cp $ORIGINAL_MAKEFILE $ORIGINAL_MAKEFILE.bkp

echo "Compiling mutants with optimisation level $FLAG..."

EXEC_DIR=$APP_RUN_DIR/COMPILED/$run

source $MASS/FAQAS-CompileOptimizedMutants/compile_optimized_mutants.sh $PROJ $PROJ_SRC $PROJ_BUILD $PROJ_ORIGINAL_BUILD $FLAG $MUTANTS_DIR $COMPILED $EXEC_DIR "${TCE_COMPILE_CMD[*]}"

rm $PROJ_ORIGINAL_BUILD

cd $PROJ
eval "${CLEAN_CMD[@]}"

mv $ORIGINAL_MAKEFILE.bkp $ORIGINAL_MAKEFILE
rm -rf $PROJ/original_build

