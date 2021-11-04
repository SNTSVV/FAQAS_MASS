#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

import sys
import re
from string import Template

# inputs
target_folder = sys.argv[1]
target_file = sys.argv[2]
probe_structure = sys.argv[3]

standard_probe = 'mutate_FM_' + '$fault_model' + '(' + '$buffer' + ' );'
probe_template = Template(standard_probe)
modified_file_content = ''

if probe_structure != 'standard':
    with open(probe_structure) as template_file:
        custom_probe = template_file.read()
        probe_template = Template(custom_probe)

with open(target_folder + '/' + target_file, 'rt') as file_to_modify:
    modified_file_content = file_to_modify.read()

    # print('****************** file before ******************')
    # print(modified_file_content)
    # print('*************************************************')

    list_of_probes = re.findall("mutation_probe(.*)", modified_file_content)
    # print(list_of_probes)
    for probe_data in list_of_probes:
        fault_model = re.findall("\((.*),.*\)", probe_data)[0]
        buffer = re.findall("\(.*,(.*)\)", probe_data)[0]
        # print(fault_model)
        # print(buffer)
        complete_probe = probe_template.substitute(fault_model=fault_model, buffer=buffer)
        # print('****************** complete_probe *******************')
        # print(complete_probe)
        comment_to_replace = '\/\/ mutation_probe\('+fault_model+','+buffer+'\)'
        # print('****************** comment_to_replace ***************')
        # print(comment_to_replace)
        # modified_file_content.replace(comment_to_replace, complete_probe)
        modified_file_content = re.sub(comment_to_replace, complete_probe, modified_file_content)
        # print('****************** file after *******************')
        # print(modified_file_content)
        # print('*************************************************')

with open(target_folder + '/' + target_file, 'wt') as file_to_modify:
    file_to_modify.write(modified_file_content)
