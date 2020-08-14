#!/bin/bash

HOME=/home/svf
TST_FOLDER=$HOME/results

MUTANTS_FOLDER=/opt/mutations/src-mutants

PYTHON_DIR=/opt/srcirorfaqas/FAQASOptimizations/FAQASPrioritization
PYTHON=/usr/bin/python3.6

prioritize=./prioritize.sh

strategy="s2"
method="euclidean"
casestudy="esail"

#pattern=adem_GetMagneticFieldCalibrated.mut.const_for_1019.77.const.adem_SetMagneticFieldCalibrationCoefficients.c
#pattern=adem_GetMagneticFieldCalibrated.mut.const_for_1104.79.const.adem_SetMagneticFieldCalibrationCoefficients.c
#pattern=adem_GetMagneticFieldCalibrated.mut.stmnt_for_94.156.delete.adem_SetMagneticFieldCalibrationCoefficients.c
#pattern=adem_GetMagneticFieldCalibrated.mut.binaryop_for_16.210.60.adem_GetMagneticFieldCalibrated.c
pattern=PDD_AIX_handler.mut.const_for_1990.508.const.AIX_ping.c
i=0
#for mutant in `find $MUTANTS_FOLDER -name '*.c'`;do
for mutant in `find $MUTANTS_FOLDER -name "$pattern"`;do   
    echo $mutant
    source $prioritize $PYTHON_DIR $PYTHON $TST_FOLDER $MUTANTS_FOLDER $strategy $method $mutant $casestudy
    if [ $i -ge 30 ];then
        break
    else
        i=$((i+1))
    fi
done
