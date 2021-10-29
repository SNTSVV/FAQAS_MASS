#!/usr/bin/env python3.7

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

import os, sys
import argparse
import subprocess
from mass_sampling import MASSSampling
from enum import Enum

parser = argparse.ArgumentParser()
parser.add_argument('--sampling', type=str)
parser.add_argument('--mut_exec_dir', type=str)
parser.add_argument('--reduced', type=str, nargs='?')

args = parser.parse_args()

sampling = args.sampling
mut_exec_dir = args.mut_exec_dir
reduced = args.reduced

sampled_mutants_file = os.path.join(mut_exec_dir, "sampled_mutants")
traces_mutants_file = os.path.join(mut_exec_dir, "main.csv")
results_mutants_file = os.path.join(mut_exec_dir, "results.csv")
error_mutants_file = os.path.join(mut_exec_dir, "results_calibration.csv")

class ContinueExecution(Enum):
    STOP = 0
    CONTINUE_FULL = 1
    CONTINUE_RED = 2

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def continue_full_ts():
    mass_sampling = MASSSampling(results_mutants_file, sampling, file_len(sampled_mutants_file), reduced)

    count = file_len(results_mutants_file)

    if mass_sampling.stopping_criterion(count) == 1:
        return ContinueExecution.STOP.value
    else:
        return ContinueExecution.CONTINUE_FULL.value

def continue_reduced_ts():
    fsci = MASSSampling(results_mutants_file, sampling, file_len(sampled_mutants_file), reduced)

    count = file_len(results_mutants_file)

    if count < fsci.fsci_calibration:
        # not enough for calibration
        return ContinueExecution.CONTINUE_FULL.value
    else:
        fsci.setFsciError(error_mutants_file)

        if fsci.stopping_criterion(count) == 1:
            return ContinueExecution.STOP.value
        else:
            return ContinueExecution.CONTINUE_RED.value if fsci.delta < fsci.tolerated_error else ContinueExecution.CONTINUE_FULL.value

if __name__ == '__main__':

    if reduced:
        print(continue_reduced_ts())
    else:
        print(continue_full_ts())

