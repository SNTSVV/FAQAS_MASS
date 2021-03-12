#!/usr/bin/env python3.7

import os, sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--coverage_file', type=str)
parser.add_argument('--result_file', type=str)

args = parser.parse_args()

coverage_file = args.coverage_file
result_file = args.result_file

def load_coverage():
    cov_dict = {}
    
    with open(coverage_file) as f:
        for line in f:
            source, covered, existing = line.strip().split(':')
            
            cov_dict[source] = covered + ":" + existing

    return cov_dict


def get_max_lines_covered(cov_dict):
    max_value = 0
    for value in cov_dict.values():
        covered, existing = value.split(":")

        if int(covered) > max_value:
            max_value = int(covered)
    
    return max_value

def get_min_lines_covered(cov_dict):
    min_value = 10000000
    for value in cov_dict.values():
        covered, existing = value.split(":")

        if int(covered) < min_value:
            min_value = int(covered)
    
    return min_value

def get_total_lines_exist(cov_dict):
    total = 0
    for value in cov_dict.values():
        covered, existing = value.split(":")
    
        total += int(existing)

    return total

def get_total_lines_covered(cov_dict):
    total_cov = 0
    for value in cov_dict.values():
        covered, existing = value.split(":")
    
        total_cov += int(covered)

    return total_cov


if __name__ == '__main__':
    
    coverage_dict = load_coverage()
    
    lines_covered = get_total_lines_covered(coverage_dict)
    lines_existing = get_total_lines_exist(coverage_dict)
    min_lines_cov = get_min_lines_covered(coverage_dict)
    max_lines_cov = get_max_lines_covered(coverage_dict)
    st_coverage = float(100*lines_covered/lines_existing)

    results = open(result_file, 'a+')
    results.write(str(lines_covered) + ";" + str("%.2f" % st_coverage) + ";" + str(min_lines_cov) + ";" + str(max_lines_cov) + "\n")
    results.close()
