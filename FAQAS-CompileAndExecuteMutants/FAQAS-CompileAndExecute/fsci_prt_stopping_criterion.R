#!/usr/bin/env Rscript

library(binom); 

oracleSTOP <- function( killed, totalRuns, l_err, h_err){
    r=binom.confint(killed,totalRuns,conf.level=CL,method="exact");
    lower = r["1","lower"] + l_err;
    higher = r["1","upper"] + h_errr;
    delta = higher-lower; 
    mean = r["", ""];

    result = list()

    if (length(delta) == 0){
        result[1] = 0
    } else if ( delta < 0.1 ) {
        result[1] = 1;
        result[2] = mean;
    } else {
        result[1] = 0;
  }
  return (result);
}

args = commandArgs(trailingOnly=TRUE)
if (length(args)==0) {
  stop("At least one argument must be supplied (input file).csv", call.=FALSE)
}

#input is a csv file
#each column reports the test suite result for a mutant: 1 means killed, 0 means not killed

executionsFile=args[1]
delta=as.numeric(args[2])
lower=as.numeric(args[3])
higher=as.numeric(args[4])

#assumption: we have a same number of mutants for every case study, otherwise leave an empty cell
mutantsExecutions=read.csv(executionsFile, sep = ";", header=FALSE)

totalMutants=nrow(mutantsExecutions)

mutationScore <- 0.0 

CL<<-0.95

terminate=0
killed=0

for( mutant in 1:totalMutants ){
    result=mutantsExecutions[mutant]

    if( result == 1 ){ #killed
        killed=killed+1        
    }

    terminate=oracleSTOP(killed, mutant, lower, higher)

    if ( terminate[[1]] == 1 ){
        score = terminate[[2]] + delta
        print(score)
        break
    }
}

cat(terminate[[1]])
