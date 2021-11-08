#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

target_folder=$1
target_file=$2

#restore the file
cp $target_folder/"$target_file".bkp $target_folder/$target_file
