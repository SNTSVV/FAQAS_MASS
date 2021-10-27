
#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

import csv
import sys


def RepresentsInt(s):
    try:
        int(s)
        return True
    except ValueError:
        return False


final_mutants_table = sys.argv[1]
working_folder = sys.argv[2]

mutation_score = 0
fault_model_coverage = 0
operator_coverage = 0

with open(str(final_mutants_table), 'r') as table, open((str(working_folder) + "/mutation_sum_up.csv"), 'w') as sum_up:
    table_reader = csv.reader(table, delimiter=',')
    mutants = set()
    covered = set()
    applied = set()
    killed = set()

    all_fault_models = set()
    covered_fault_models = set()

    # MutationOpt,FaultModel,DataItem,Span,Type,FaultClass,Min,Max,Threshold,Delta,State,Value,Status,Application
    next(table_reader)

    for line in table_reader:
        mutant = line[0]
        fm = line[1]
        status = line[12]
        application = line[13]

        mutants.add(mutant)
        all_fault_models.add(fm)

        if status != "NOT_COVERED":
            covered_fault_models.add(fm)
            covered.add(mutant)
            if application == "APPLIED":
                applied.add(mutant)
                if status != "LIVE" and status != "NOT_COVERED":
                    killed.add(mutant)
                else:
                    continue
            else:
                continue
        else:
            continue

    outwrite = csv.writer(sum_up, delimiter=',')

    FM_nr = len(all_fault_models)
    covered_FM_nr = len(covered_fault_models)

    fault_model_coverage = float(covered_FM_nr)/float(FM_nr)

    print("you can find this results at:")
    print(str(working_folder) + "/mutation_sum_up.csv")

    outwrite.writerow(["all_fault_models", "covered_fault_models", "fault_model_coverage"])
    print(["all_fault_models", "covered_fault_models", "fault_model_coverage"])
    outwrite.writerow([FM_nr, covered_FM_nr, round(fault_model_coverage, 3)])
    print([FM_nr, covered_FM_nr, round(fault_model_coverage, 3)])

    covered_nr = len(covered)
    applied_nr = len(applied)

    operator_coverage = float(applied_nr)/float(covered_nr)

    outwrite.writerow(["covered_mutants", "applied_mutants", "operator_coverage"])
    outwrite.writerow([covered_nr, applied_nr, round(operator_coverage, 3)])
    print(["covered_mutants", "applied_mutants", "operator_coverage"])
    print([covered_nr, applied_nr, round(operator_coverage, 3)])

    killed_nr = len(killed)
    mutation_score = float(killed_nr)/float(applied_nr)

    outwrite.writerow(["applied_mutants", "killed_mutants", "mutation_score"])
    outwrite.writerow([applied_nr, killed_nr, round(mutation_score, 3)])
    print(["applied_mutants", "killed_mutants", "mutation_score"])
    print([applied_nr, killed_nr, round(mutation_score, 3)])

print("more details are avalaible in the .csv files contained in the 'results' folder")
