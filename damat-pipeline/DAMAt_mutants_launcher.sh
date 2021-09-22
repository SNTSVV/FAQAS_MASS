#/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

set -e

DAMAt_FOLDER=$(pwd)

. $DAMAt_FOLDER/DAMAt_configure.sh

echo "PIPELINE_FOLDER=$PIPELINE_FOLDER"
echo "RESULTS_FOLDER=$RESULTS_FOLDER"
echo "LOGS_FOLDER=$LOGS_FOLDER"
echo "TESTS_FOLDER=$TESTS_FOLDER"
echo "DATA_ANALYSIS_FOLDER=$DATA_ANALYSIS_FOLDER"
echo "mutator=$mutator"
echo "mutants_table=$mutants_table"


if [ $singleton == "TRUE" ]; then
export _FAQAS_SINGLETON_FM=$singleton
echo "******************* SINGLETON MODE ******************"
fi

###############################################################################
# STEP 4: COMPILE ALL MUTANTS
# STEP 5: RUN ALL MUTANTS


if [ -f $RESULTS_FOLDER ]; then
  results_now=previous_results_$(date +"%m_%d_%y_%H-%M")
  mv $RESULTS_FOLDER $results_now
fi

mkdir $RESULTS_FOLDER

mkdir $LOGS_FOLDER

# OBTAIN COVERAGE DATA
mkdir $TESTS_FOLDER
bash $PIPELINE_FOLDER/DAMAt_obtain_coverage.sh $DAMAt_FOLDER

# COMPILE AND RUN ALL MUTANTS
bash $PIPELINE_FOLDER/DAMAt_compile_and_run_mutants.sh $DAMAt_FOLDER

###############################################################################

# STEP 6: GENERATE THE RESULTS OF THE MUTATION ANALYSIS

bash $PIPELINE_FOLDER/DAMAt_data_analysis.sh $DAMAt_FOLDER

###############################################################################
