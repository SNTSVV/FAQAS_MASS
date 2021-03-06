#!/usr/bin/env Rscript

#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

library(binom); 

oracleSTOP <- function( killed, totalRuns ){
    r = binom.confint(killed,totalRuns,conf.level=CL,method="exact");
    lower = r["1","lower"];
    higher = r["1","upper"];
    delta = higher-lower; 

    if ( delta < 0.10 ) {
        res=1;
    } else {
        res=0;
    }
    return (res);
}

args = commandArgs(trailingOnly=TRUE)
if (length(args)==0) {
  stop("At least one argument must be supplied (input file).csv", call.=FALSE)
}

#input is a csv file
#each column reports the test suite result for a mutant: 1 means killed, 0 means not killed

executionsFile=args[1]

#assumption: we have a same number of mutants for every case study, otherwise leave an empty cell
mutantsExecutions=read.csv(executionsFile, sep = ";", header=FALSE)

totalMutants=nrow(mutantsExecutions)

#vector with the mutation score for each run
mutationScore <- 0.0

CL<<-0.95

terminate=0
killed=0
  
for( mutant in 1:totalMutants ){
    result=mutantsExecutions[mutant,]
      
    if( result == 1 ){ #killed
        killed=killed+1        
    }
      
    terminate=oracleSTOP(killed, mutant)
      
    if ( terminate == 1 ){

#        score = killed/mutant
#        mutationScore=score
       break
    }
}

cat(terminate)
