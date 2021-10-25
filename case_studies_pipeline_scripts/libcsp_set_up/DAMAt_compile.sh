#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
# Modified by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

mutant_id=$1
singleton=$2

###############################################################################
#enabling extended pattern matching features:
shopt -s
#options for enabling aliases:
shopt -s expand_aliases
###############################################################################


echo "------------------------------------"
echo "------------------------------------"
echo "Mutant opt: "$mutant_id
echo "------------------------------------"
echo "------------------------------------"

###############################################################################

#exporting the operation counter
export MUTATIONOPT=$mutant_id

if [ $singleton == "TRUE" ]; then
export _FAQAS_SINGLETON_FM=$singleton
echo "******************* SINGLETON MODE ******************"
fi

# here the user must invoke the compilation of the SUT, we provided a simple example.


TEST_FOLDER="/home/csp/libparam/lib/libgscsp/tst"

pushd $TEST_FOLDER

for f in *; do
    if [ -d "$f" ] && [ "$f" != "include" ]; then

        pushd $f
        echo "cleaning..."
        ./waf clean

        echo "configuring..."
        ./waf configure --mutation-opt $mutant_id --singleton TRUE

        if [ $? -eq 0 ]; then
            echo $x " compilation OK"
        else
            echo $x " compilation FAILED"
        fi
        popd
    fi
done

popd

