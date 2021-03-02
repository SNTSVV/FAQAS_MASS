#!/bin/bash

export PROJ_ORIGINAL_BUILD=$PROJ/original_build/$COMPILED

# backing up original makefile
cp $ORIGINAL_MAKEFILE $ORIGINAL_MAKEFILE.bkp

run=1
for FLAG in "${FLAGS[@]}";do
    echo "Compiling mutants with optimisation level $FLAG..."

    EXEC_DIR=$APP_RUN_DIR/COMPILED/$run

   source $MASS/FAQAS-CompileOptimizedMutants/compile_optimized_mutants.sh $PROJ $PROJ_SRC $PROJ_BUILD $PROJ_ORIGINAL_BUILD $FLAG $MUTANTS_DIR $COMPILED $EXEC_DIR "${TCE_COMPILE_CMD[*]}"

    rm $PROJ_ORIGINAL_BUILD

    cd $PROJ
    "${CLEAN_CMD[@]}"

    run=$((run+1))
done

mv $ORIGINAL_MAKEFILE.bkp $ORIGINAL_MAKEFILE
rm -rf $PROJ/original_build

source $MASS/FAQAS-CompileOptimizedMutants/FAQAS-PostProcessing.sh $APP_RUN_DIR/COMPILED


