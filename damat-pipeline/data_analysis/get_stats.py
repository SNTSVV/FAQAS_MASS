
#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

import csv
import sys
from collections import defaultdict


#python3 $DATA_ANALYSIS_FOLDER/get_stats.py
# $readable_results= readable_data.csv
# $readable_operator_coverage= readable_operator_coverage.csv
# $readable_FM_coverage readable_FM_coverage.csv
# $mutants_table

def RepresentsInt(s):
    try:
        int(s)
        return True
    except ValueError:
        return False


data_file = sys.argv[1]
operator_coverage_data = sys.argv[2]
FM_coverage_data = sys.argv[3]
mutants_table = sys.argv[4]
working_folder = sys.argv[5]

with open(str(data_file), 'r') as all_data, open(str(FM_coverage_data)) as FM_coverage, open(str(operator_coverage_data)) as operator_coverage:

    all_mutants = set()
    killed_mutants = set()
    live_mutants = set()

    covered_fault_models = set()
    covered_mutants = set()

    operator_covered_mutants = set()

    fault_models = defaultdict(set)
    fault_classes = defaultdict(set)
    data_items = defaultdict(set)

    covered_tests_per_fault_model = defaultdict(set)

    FM_coverage_table = csv.reader(FM_coverage, delimiter=',')
    # Test,FaultModel,Status,Times

    for cov_row in FM_coverage_table:
        if cov_row[2] == 'covered':
            covered_fault_models.add(cov_row[1])
            covered_tests_per_fault_model[str(cov_row[1])].add(cov_row[0])
    operator_coverage_table = csv.reader(operator_coverage, delimiter=',')

    # Operator,TestCase,Calls,Mutations

    op_coverage = defaultdict(list)

    for op_row in operator_coverage_table:
        cov_operator = op_row[0]
        mutations_performed = op_row[3]
        calls = op_row[2]
        if cov_operator not in op_coverage and RepresentsInt(cov_operator):
            op_coverage[str(cov_operator)] = [mutations_performed, calls]
        elif cov_operator in op_coverage and RepresentsInt(cov_operator):
            values = op_coverage[str(cov_operator)]
            new_values = [0, 0]
            new_values[0] = int(values[0]) + int(mutations_performed)
            new_values[1] = int(values[1]) + int(calls)
            op_coverage[str(cov_operator)] = [new_values[0], new_values[1]]

    for operator in op_coverage:
        operator_info = op_coverage[operator]
        if operator_info[0] != 0:
            operator_covered_mutants.add(operator)

    # 0 mutant 1 compilation 2 test 3 result 4 status 5 time 6 FaultModel
    # 7 DataItemn 8 Span 9 Type 10 FaultClass 11 Min 12 Max 13 Threshold
    # 14 Delta 15 State 16 Value

    data_table = csv.reader(all_data, delimiter=',')

    for row in data_table:
        mutant = row[0]
        status = row[4]
        fault_class = row[10]
        fault_model = row[6]
        data_item = row[7]

        if fault_class != "NA":

            fault_classes[str(fault_class)].add(mutant)
            fault_models[str(fault_model)].add(mutant)
            data_items[str(data_item)].add(mutant)

            if RepresentsInt(mutant) and int(mutant) >= 0:
                all_mutants.add(mutant)

            if str(status) == 'KILLED':
                killed_mutants.add(mutant)

            if str(status) == 'LIVE':
                live_mutants.add(mutant)
            if fault_model in covered_fault_models:
                covered_mutants.add(mutant)

    covered_and_killed_mutants = killed_mutants.intersection(covered_mutants)


    all_covered_mutants = all_mutants.intersection(covered_mutants)

    number_of_killed_mutants = float(len(covered_and_killed_mutants))



    total_number_of_mutants = float(len(all_covered_mutants))


    mutation_score = round((number_of_killed_mutants/total_number_of_mutants)*100, 3)


    operator_covered_and_killed = covered_and_killed_mutants.intersection(operator_covered_mutants)

    operator_covered_mutants = all_covered_mutants.intersection(operator_covered_mutants)

    number_of_killed_mutants_op = float(len(operator_covered_and_killed))

    total_number_of_mutants_op = float(len(operator_covered_mutants))

    mutation_score_operators = round(
        (number_of_killed_mutants_op/total_number_of_mutants_op)*100, 3)


    with open(str(working_folder) + "/test_coverage.csv", 'w') as detailed_coverage_table:
        cov_table_writer = csv.writer(detailed_coverage_table, delimiter=',')
        for FM in covered_tests_per_fault_model:
            list_of_test = list(covered_tests_per_fault_model[FM])
            list_of_test.insert(0, str(FM))
            cov_table_writer.writerow(list_of_test)
    detailed_coverage_table.close()

    # print(fault_models)
    with open(str(working_folder) + "/mutation_score_by_fault_model.csv", 'w') as mut_score_fm_out:
        fm_writer = csv.writer(mut_score_fm_out, delimiter=',')
     #   fm_row = ['FaultModel', 'Mutants', 'Killed', 'MutationScore']
     #   fm_writer.writerow(fm_row)
     #   fm_row = ['General', str(total_number_of_mutants), str(
     #       number_of_killed_mutants), str(mutation_score)]
     #   fm_writer.writerow(fm_row)
     #   for model in fault_models:
     #       killed_by_fm = fault_models[model].intersection(covered_and_killed_mutants)
     #       nr_killed_by_fm = len(killed_by_fm)
     #       total_number_by_fm = len(fault_models[model].intersection(all_covered_mutants))
     #       if total_number_by_fm != 0:
     #           mutation_score_by_fm = round((float(nr_killed_by_fm)/float(total_number_by_fm)) * 100, 3)
     #       else:
     #           mutation_score_by_fm = "NA"
     #       fm_row = [str(model), str(total_number_by_fm), str(
     #           nr_killed_by_fm), str(mutation_score_by_fm)]
     #       fm_writer.writerow(fm_row)

        fm_row = ['FaultModel', 'AppliedMutants', 'Killed', 'MutationScoreOperators']
        fm_writer.writerow(fm_row)
        fm_row = ['General', str(total_number_of_mutants_op), str(
            total_number_of_mutants_op), str(mutation_score_operators)]
        fm_writer.writerow(fm_row)
        for model in fault_models:
            killed_by_fm = fault_models[model].intersection(operator_covered_and_killed)
            nr_killed_by_fm = len(killed_by_fm)
            total_number_by_fm = len(fault_models[model].intersection(operator_covered_mutants))
            if total_number_by_fm != 0:
                mutation_score_by_fm = (float(nr_killed_by_fm)/float(total_number_by_fm)) * 100
            else:
                mutation_score_by_fm = "NA"
            fm_row = [str(model), str(total_number_by_fm), str(
                nr_killed_by_fm), str(mutation_score_by_fm)]
            fm_writer.writerow(fm_row)

    mut_score_fm_out.close()

    # print(fault_classes)
    with open(str(working_folder) + "/mutation_score_by_fault_class.csv", 'w') as mut_score_fc_out:
        fc_writer = csv.writer(mut_score_fc_out, delimiter=',')
      #  fc_row = ['FaultClass', 'Mutants', 'Killed', 'MutationScore']
      #  fc_writer.writerow(fc_row)
      #  fc_row = ['General', str(total_number_of_mutants), str(
      #      number_of_killed_mutants), str(mutation_score)]
      #  fc_writer.writerow(fc_row)
      #  for f_class in fault_classes:
      #      killed_by_fc = fault_classes[f_class].intersection(covered_and_killed_mutants)
      #      nr_killed_by_fc = len(killed_by_fc)
      #      total_number_by_fc = len(fault_classes[f_class].intersection(all_covered_mutants))
      #      if total_number_by_fc != 0:
      #          mutation_score_by_fc = (float(nr_killed_by_fc)/float(total_number_by_fc)) * 100
      #      else:
      #          mutation_score_by_fc = "NA"
      #      fc_row = [str(f_class), str(total_number_by_fc), str(
      #              nr_killed_by_fc), str(mutation_score_by_fc)]
      #      fc_writer.writerow(fc_row)

        fc_row = ['FaultClass', 'AppliedMutants', 'Killed', 'MutationScore']
        fc_writer.writerow(fc_row)
        fc_row = ['General', str(total_number_of_mutants_op), str(number_of_killed_mutants_op), str(mutation_score_operators)]
        fc_writer.writerow(fc_row)

        for f_class in fault_classes:
            killed_by_fc = fault_classes[f_class].intersection(operator_covered_and_killed)
            nr_killed_by_fc = len(killed_by_fc)
            total_number_by_fc = len(fault_classes[f_class].intersection(operator_covered_mutants))
            if total_number_by_fc != 0:
                mutation_score_by_fc = (float(nr_killed_by_fc)/float(total_number_by_fc)) * 100
            else:
                mutation_score_by_fc = "NA"
            fc_row = [str(f_class), str(total_number_by_fc), str(
                nr_killed_by_fc), str(mutation_score_by_fc)]
            fc_writer.writerow(fc_row)

    mut_score_fc_out.close()

    # print(data_items)
    with open(str(working_folder) + "/mutation_score_by_data_item.csv", 'w') as mut_score_di_out:
        di_writer = csv.writer(mut_score_di_out, delimiter=',')
      #  di_row = ['DataItem', 'Mutants', 'Killed', 'MutationScore']
      #  di_writer.writerow(di_row)
      #  di_row = ['General', str(total_number_of_mutants), str(
      #      number_of_killed_mutants), str(mutation_score)]
      #  di_writer.writerow(di_row)
      #  for d_item in data_items:
      #      killed_by_di = data_items[d_item].intersection(covered_and_killed_mutants)
      #      nr_killed_by_di = len(killed_by_di)
      #      total_number_by_di = len(data_items[d_item].intersection(all_covered_mutants))
      #      if total_number_by_di != 0:
      #          mutation_score_by_di = (float(nr_killed_by_di)/float(total_number_by_di)) * 100
      #      else:
      #          mutation_score_by_di = "NA"
      #      di_row = [str(d_item), str(total_number_by_di), str(
      #          nr_killed_by_di), str(mutation_score_by_di)]
      #      di_writer.writerow(di_row)

        di_row = ['DataItem', 'AppliedMutants', 'Killed', 'MutationScore']
        di_writer.writerow(di_row)
        di_row = ['General', str(total_number_of_mutants_op), str(number_of_killed_mutants_op), str(mutation_score_operators)]
        di_writer.writerow(di_row)
        for d_item in data_items:
            killed_by_di = data_items[d_item].intersection(operator_covered_and_killed)
            nr_killed_by_di = len(killed_by_di)
            total_number_by_di = len(data_items[d_item].intersection(operator_covered_mutants))
            if total_number_by_di != 0:
                mutation_score_by_di = (float(nr_killed_by_di)/float(total_number_by_di)) * 100
            else:
                mutation_score_by_di = "NA"
            di_row = [str(d_item), str(total_number_by_di), str(
                nr_killed_by_di), str(mutation_score_by_di)]
            di_writer.writerow(di_row)
    mut_score_di_out.close()

    print(covered_mutants)

    with open(str(mutants_table), 'r') as mutants_definitions, open(str(working_folder) + "/final_mutants_table.csv", 'w') as final_table:
        definitions_reader = csv.reader(mutants_definitions, delimiter=',')
        table_writer = csv.writer(final_table, delimiter=',')
        headings = next(definitions_reader)
        headings.extend(["Status", "Application"])
        table_writer.writerow(headings)
        for line in definitions_reader:
            mutation_id = line[0]
            if mutation_id in covered_and_killed_mutants:
                status = 'KILLED'
            elif mutation_id not in covered_mutants:
                status = 'NOT_COVERED'
            else:
                status = 'LIVE'

            application = "NOT_APPLIED"

            if str(mutation_id) in operator_covered_mutants:
                application = "APPLIED"

            new_line = line
            line.extend([status, application])
            table_writer.writerow(new_line)
    mutants_definitions.close()
    final_table.close()

all_data.close()
