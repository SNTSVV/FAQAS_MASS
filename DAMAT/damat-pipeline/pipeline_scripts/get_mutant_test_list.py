
#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

import csv
import sys
from collections import defaultdict

FM_coverage_data = sys.argv[1]
full_test_list = sys.argv[2]
mutants_table = sys.argv[3]
output_folder = sys.argv[4]

covered_fault_models = set()
covered_tests_per_fault_model = defaultdict(set)
give_me_the_time = {}
give_me_the_fm = {}
list_of_all_mutants = []

with open(full_test_list) as all_tests:
    test_reader = csv.reader(all_tests, delimiter=',')
    for line in test_reader:
        test_name = line[0]
        normal_time = line[1]
        give_me_the_time[test_name] = normal_time

with open(mutants_table) as mu_table:
    mu_reader = csv.reader(mu_table, delimiter=',')
    for line in mu_reader:
        mutant_name = line[0]
        fault_model = line[1]
        give_me_the_fm[mutant_name] = fault_model
        list_of_all_mutants.append(mutant_name)

with open(str(FM_coverage_data)) as FM_coverage:
    FM_coverage_table = csv.reader(FM_coverage, delimiter=',')
    for cov_row in FM_coverage_table:
        if cov_row[2] == 'covered':
            covered_fault_models.add(cov_row[1])
            covered_tests_per_fault_model[str(cov_row[1])].add(cov_row[0])

for mutant in list_of_all_mutants:
    with open(str(output_folder) + "/" + "tests_" + str(mutant), "w") as out:
        time_writer = csv.writer(out, delimiter=',')
        fm = give_me_the_fm[mutant]
        covered_tests = covered_tests_per_fault_model[fm]
        for test in covered_tests:
            time = give_me_the_time[str(test)]
            new_row = [test, time]
            time_writer.writerow(new_row)
