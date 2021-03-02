#!/usr/bin/env Rscript

library(binom); 

oracleSTOP <- function( killed, totalRuns, l_err, h_err){
    r=binom.confint(killed,totalRuns,conf.level=CL,method="exact");
    lower = r["1","lower"] + l_err;
    higher = r["1","upper"] + h_err;
    mean = r["1", "mean"];
    delta = higher-lower; 

    res = paste("lower", lower, "higher", higher, "delta", delta, sep=" ")
    
    return (res);
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

CL<<-0.95

terminate=""
killed=0

for( mutant in 1:totalMutants ){
    result=mutantsExecutions[mutant,]

    if( result == 1 ){ #killed
        killed=killed+1        
    }

    terminate=oracleSTOP(killed, mutant, lower, higher)
}

cat(terminate)
