#!/bin/bash

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#


# Both variables INSTALL_DIR and EXECUTION_DIR must be set before using this script
# workspace installation directory
# export INSTALL_DIR=

if ! env | grep -q ^INSTALL_DIR;then echo "env variable INSTALL_DIR must be set" && exit 1;fi

# if MASS will run inside a container, specify where it will be located inside the container
# otherwise, leave empty
# export EXECUTION_DIR=

if [ -z $EXECUTION_DIR ];then
    EXECUTION_DIR=$INSTALL_DIR
fi

mkdir -p $INSTALL_DIR

cp CompileOptimizedMutants.sh $INSTALL_DIR
cp OptimizedPostProcessing.sh $INSTALL_DIR
cp GenerateMutants.sh $INSTALL_DIR
cp GeneratePTS.sh $INSTALL_DIR
cp ExecuteMutants.sh $INSTALL_DIR
cp IdentifyEquivalents.sh $INSTALL_DIR
cp PrepareSUT.sh $INSTALL_DIR
cp mass_conf.sh $INSTALL_DIR
cp mutation_additional_functions.sh $INSTALL_DIR
cp PrepareMutants_HPC.sh $INSTALL_DIR
cp ExecuteMutants_HPC.sh $INSTALL_DIR
cp PostMutation_HPC.sh $INSTALL_DIR
cp MutationScore.sh $INSTALL_DIR
cp Launcher.sh $INSTALL_DIR

cd $INSTALL_DIR

sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" CompileOptimizedMutants.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" OptimizedPostProcessing.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" GenerateMutants.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" GeneratePTS.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" ExecuteMutants.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" IdentifyEquivalents.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" PrepareSUT.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" PrepareMutants_HPC.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" ExecuteMutants_HPC.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" PostMutation_HPC.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" MutationScore.sh
sed -i "s:INSTALL_DIR_REPL:$EXECUTION_DIR:g" Launcher.sh

mkdir -p MASS_STEPS_LAUNCHERS

mv CompileOptimizedMutants.sh OptimizedPostProcessing.sh GenerateMutants.sh GeneratePTS.sh ExecuteMutants.sh IdentifyEquivalents.sh PrepareSUT.sh PrepareMutants_HPC.sh ExecuteMutants_HPC.sh PostMutation_HPC.sh MutationScore.sh MASS_STEPS_LAUNCHERS/

