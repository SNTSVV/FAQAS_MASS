#!/usr/bin/env Rscript

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

library(binom);

computeError <- function( resultA, resultB ){
    mismatches <- resultA != resultB
#    print(mismatches)
      
    CL<<-0.95 
  
    totalMutants=nrow(resultA)
    # print(totalMutants)
    nMismatches = sum(mismatches == 1)
    
    r=binom.confint(nMismatches,totalMutants,conf.level=CL,method="wilson");
  
    lower=r["1","lower"];
    higher=r["1","upper"];
  
    delta = higher-lower; 
    
    returnValue = paste(delta, lower, higher, sep=";")
    return (returnValue);
}

args = commandArgs(trailingOnly=TRUE)
if (length(args)==0) {
    stop("At least two arguments must be supplied (input file).csv", call.=FALSE)
}

#input is a csv file
#each column reports the test suite result for a mutant: 1 means killed, 0 means not killed

prioritizedTSFile=args[1]
fullTSFile=args[2]

full_results = read.csv(fullTSFile, header = FALSE, sep = ";", dec = ".")
prioritized_results = read.csv(prioritizedTSFile, header = FALSE, sep = ";", dec = ".")

error = computeError(full_results, prioritized_results)

cat(error)
