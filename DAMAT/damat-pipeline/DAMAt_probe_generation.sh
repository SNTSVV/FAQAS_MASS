#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#


DAMAt_FOLDER=$(pwd)

. $DAMAt_FOLDER/DAMAt_configure.sh

# _FAQAS_SINGLETON_FM="TRUE" can be exported to load the fault model in a singleton variable to save memory
if [ $singleton == "TRUE" ]; then
export _FAQAS_SINGLETON_FM=$singleton
echo "******************* SINGLETON MODE ******************"
fi

export _FAQAS_INITIAL_PADDING=$padding
###############################################################################

# STEP 2 MUTATION API GENERATION
pushd $DAMAt_FOLDER

python3 generateDataMutator.py "$buffer_type" "$fault_model"
popd

###############################################################################
