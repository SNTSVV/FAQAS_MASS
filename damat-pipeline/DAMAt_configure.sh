#!/bin/sh

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

DAMAt_FOLDER=$(pwd)

# the user must set up the following variables before executing the pipeline:
# the location of the csv with all the test identifiers and the normal execution time
tests_list=$DAMAt_FOLDER/tests.csv
# the location of the csv containing the definitions of the mutation operators
fault_model=$DAMAt_FOLDER/fault_model.csv
# the datatype of the elements of the target buffer
buffer_type="unsigned char"
# padding can be used to skip the first n bit of a buffer, normally set to 0
padding=2
