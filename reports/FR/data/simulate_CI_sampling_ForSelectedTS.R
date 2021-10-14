#library(scatterplot3d)
#library(plot3D)
#library(binGroup); 
library(binom); 
#library(rgl)


#This function receives two vectors with equal size. Each element represent the mutation results for a mutant. 
#resultA are the results obtained with the full test suite
#resultB are the results obtained with the reduced test suite
#It returns the max deviation to be expected from the real mutation score.
computeError <- function( resultA, resultB ){
  mismatches <- resultA != resultB
  print(resultA)
  print(resultB)
  print(mismatches)
  
  
  
  totalMutants=length(resultA)
  
  
  nMismatches = sum(mismatches == 1)
    
  r=binom.confint(nMismatches,totalMutants,conf.level=CL,method="wilson");
  
  print(r)
  
  lower=r["1","lower"];
  higher=r["1","upper"];
  mean=r["1","mean"];
  
  delta = higher-lower; 

	#return (mean+delta/2);
  return (higher);
}

args = commandArgs(trailingOnly=TRUE)
if (length(args)==0) {
  stop("At least one argument must be supplied (input file).csv", call.=FALSE)
}

#input is a csv file
#each column reports the test suite result for a mutant: 1 means killed, 0 means not killed

executionsFile=args[1]

#assumption: we have a same number of mutants for every case study, otherwise leave an empty cell
mutantsExecutions=read.csv(executionsFile,sep = ";",header = TRUE)



CL<<-0.95

NSIM=100
totError=0
for ( simulation in 1:NSIM){
  killed=0
  
  mutantsExecutionsShuffled <- mutantsExecutions[sample(nrow(mutantsExecutions)),]
  
  
  
  resultALL = mutantsExecutionsShuffled[c(1:100),2]
  resultSelected = mutantsExecutionsShuffled[c(1:100),3]
  
  
  error=computeError(resultALL,resultSelected)
  
  totError=error+totError
  
  
}

print(totError/NSIM)
