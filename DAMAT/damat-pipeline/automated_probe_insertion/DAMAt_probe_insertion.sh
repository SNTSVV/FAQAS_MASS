#!/bin/bash

#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

target_folder=$1
target_file=$2
template=$3
header_path=$4

#back up the file
cp $target_folder/$target_file $target_folder/"$target_file".bkp

#substitute the probes
python3 DAMAt_insert_probes.py $target_folder $target_file $template $header_path
