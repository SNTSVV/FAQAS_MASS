#/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

set -e

DAMAt_FOLDER=$(pwd)

. ./DAMAt_configure.sh

#_FAQAS_INITIAL_PADDING can be exported to skip the first n bit of a buffer
export _FAQAS_INITIAL_PADDING=$padding
# _FAQAS_SINGLETON_FM="TRUE" can be exported to load the fault model in a singleton variable to save memory
export _FAQAS_SINGLETON_FM="TRUE"

###############################################################################
# STEP 4: COMPILE ALL MUTANTS
# STEP 5: RUN ALL MUTANTS

PIPELINE_FOLDER=$DAMAt_FOLDER/pipeline_scripts
RESULTS_FOLDER=$DAMAt_FOLDER/results
TESTS_FOLDER=$DAMAt_FOLDER/testlists


if [ -f $RESULTS_FOLDER ]; then
  results_now=previous_results_$(date +"%m_%d_%y_%H-%M")
  mv $RESULTS_FOLDER $results_now
fi

mkdir $RESULTS_FOLDER

LOGS_FOLDER=$RESULTS_FOLDER/logs
mkdir $LOGS_FOLDER

# OBTAIN COVERAGE DATA
mkdir $TESTS_FOLDER
bash $PIPELINE_FOLDER/DAMAt_obtain_coverage.sh $tests_list $DAMAt_FOLDER

# COMPILE AND RUN ALL MUTANTS
bash $PIPELINE_FOLDER/DAMAt_compile_and_run_mutants.sh $DAMAt_FOLDER

###############################################################################

# STEP 6: GENERATE THE RESULTS OF THE MUTATION ANALYSIS

bash $PIPELINE_FOLDER/DAMAt_data_analysis.sh $DAMAt_FOLDER $tests_list

###############################################################################
