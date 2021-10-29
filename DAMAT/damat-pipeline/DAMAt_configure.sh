#!/bin/sh
#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#
DAMAt_FOLDER=$(pwd)
PIPELINE_FOLDER=$DAMAt_FOLDER/pipeline_scripts
RESULTS_FOLDER=$DAMAt_FOLDER/results
LOGS_FOLDER=$RESULTS_FOLDER/logs
TESTS_FOLDER=$DAMAt_FOLDER/testlists
DATA_ANALYSIS_FOLDER=$DAMAt_FOLDER/data_analysis
mutator=$DAMAt_FOLDER/FAQAS_dataDrivenMutator.h
mutants_table=$DAMAt_FOLDER/FAQAS_mutants_table.csv

# the user must set up the following variables before executing the pipeline:

# the location of the csv with all the test identifiers and the execution time
tests_list=$DAMAt_FOLDER/tests.csv

# the location of the csv containing the definitions of the mutation operators
fault_model=$DAMAt_FOLDER/fault_model.csv

# the datatype of the elements of the target buffer
buffer_type="unsigned long long int"

# padding: can be used to skip the first n bit of a buffer, normally set to 0
padding=0

# singleton: can set to true to load the fault model into a singleton variable, normally set to "TRUE", can also  be set to "FALSE"
singleton="TRUE"

if [ $singleton == "TRUE" ]; then
export _FAQAS_SINGLETON_FM=$singleton
fi
