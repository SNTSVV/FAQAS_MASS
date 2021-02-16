#!/usr/bin/env Rscript

library(binom); 

oracleSTOP <- function( killed, totalRuns, error ){
    r=binom.confint(killed,totalRuns,conf.level=CL,method="exact");
    lower=r["1","lower"];
    higher=r["1","upper"] + error;
    delta = higher-lower; 
    o="";  

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
error=as.numeric(args[2])

#assumption: we have a same number of mutants for every case study, otherwise leave an empty cell
mutantsExecutions=read.csv(executionsFile, sep = ";", header=FALSE)

totalMutants=nrow(mutantsExecutions)

totalRuns=ncol(mutantsExecutions)

#vector with the number of mutants executed for each run
mutantsSimulatedPerRun <- rep(NA, totalRuns)

#vector with the mutation score for each run
mutationScore <- rep(NA, totalRuns)

CL<<-0.95

terminate=0
for ( run in 1:totalRuns){
  killed=0
  
  for( mutant in 1:totalMutants ){
      result=mutantsExecutions[mutant,run]
      
      if( result == 1 ){ #killed
        killed=killed+1        
      }
      
      terminate=oracleSTOP(killed, mutant, error)
      
      if ( terminate == 1 ){

        score = killed/mutant
#        print(score)
        mutationScore[run]=score
        mutantsSimulatedPerRun[run]=mutant
        break
      }
  }  
}

cat(terminate)
