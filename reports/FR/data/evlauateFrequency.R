library(EnvStats)
library(robustbase)
library(MESS)

N=1000



COA_300 = FALSE
COA_400 = FALSE
COA_1000 = FALSE

COMPUTE_OPTIMAL_R = TRUE


FREQ <<- matrix(NA, nrow=101, ncol=10)
COUNT <<- matrix(NA, nrow=101, ncol=10)

evalHist <- function(file,CS){
  
  data <- read.csv(file, header = FALSE, sep=";" )
  
  b<-c(0:101)/100-0.01
  
  d<-as.numeric(unlist(data[,5]))
  
  pos <- which(d>1)
  
  d[pos]<-NA
  
  d <- na.omit(d)
  
  h <- hist(d,breaks=b,plot=FALSE,right=TRUE,)
  
  
  
  COUNT[,CS] <<- h$counts
  
  
  FREQ[,CS] <<- h$density
}

printPlot<- function( fdest, data, ytitle="% of mutants" ){
  #fdest = paste0(file,suffix,"_density.pdf")
  
  
  
  pdf(file = fdest)
  
  
  legendNames=c("ESAIL","LIBN","LIBP","LIBU", "MLFS")
  selectedColors=c("black","red","blue","purple","darkgreen")
  
  topY=0.06
  
  x <- c(0:100)/100
  
  plot(x, data[,1],col=selectedColors[1], xlim=c(0, 1), ylim=c(0,100), xlab="", ylab="", lty=1, main="", cex.lab=1.5, cex.axis=1.5, type="l")
  
  title(main = "", sub = "", xlab = "Distance", ylab = ytitle, line = NA, outer = FALSE, cex.sub=1.5, cex.main=1.5, cex.lab=1.5 )
  
  xtick<-c(0:20)/100*5
  axis(side=1, at=xtick, labels = FALSE)
  
  # Create a sample of 50 numbers which are incremented by 1.
  

  b = points(x, data[,2], col=selectedColors[2], type="l" )
  
  b = points(x, data[,3], col=selectedColors[3], type="l" )
  # 
  b = points(x, data[,4], col=selectedColors[4], type="l" )
  # 
  b = points(x, data[,5], col=selectedColors[5], type="l" )
  
  for ( i in (5:9) ){
    b = points(x, data[,i], col=selectedColors[(i%%5)+1], type="l", lty=3 )
  }

  legend(0.2,50,legendNames,
         #col=selectedColors,
         lty=c(1,1,1,1,1),
         col=selectedColors,
         #fill=selectedColors,
         border=selectedColors,horiz=F, title="ALL")
  
  legend(0.7,50,legendNames,
         #col=selectedColors,
         lty=c(2,2,2,2,2),
         col=selectedColors,
         #fill=selectedColors,
         border=selectedColors,horiz=F, title="MASS")
  
  
  dev.off()
}

analyzeAndPrint<-function(file, ytitle="% of Mutants"){
  print("Counts")
  print (COUNT)
  
  print("Density")
  print (FREQ)
  for ( j in (1:100) ){
    a <-FREQ[(j+1),]+FREQ[j,]
    print(a)
    FREQ[(j+1),] <<- a
    
    
    a <-COUNT[(j+1),]+COUNT[j,]
    print(a)
    COUNT[(j+1),] <<- a
  }
  
  print("Cumulative Density")
  print (FREQ)
  
  print("Cumulative Count")
  print (COUNT)
  printPlot(file,FREQ,ytitle)
}

EQUIVALENT=TRUE
if(EQUIVALENT){
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7/filtered_equivalents_esail.csv",1)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7/filtered_equivalents_csp.csv",2)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7/filtered_equivalents_param.csv",3)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7/filtered_equivalents_gsl.csv",4)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7/filtered_equivalents_mlfs.csv",5)

evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/filtered_equivalents_esail.csv",6)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/filtered_equivalents_csp.csv",7)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/filtered_equivalents_param.csv",8)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/filtered_equivalents_libutil.csv",9)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/filtered_equivalents_mlfs.csv",10)

analyzeAndPrint("distanceFequency_Equivalent.pdf")

}



REDUNDANT=TRUE
if(REDUNDANT){
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7/redundants_esail.csv",1 )
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7/redundants_libcsp.csv",2 )
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7/redundants_libparam.csv",3)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7/redundants_libutil.csv",4)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7/redundants_mlfs.csv",5)

evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/redundants_esail_red.csv",6)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/redundants_csp_red.csv",7)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/redundants_param_red.csv",8)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/redundants_libutil_red.csv",9)
evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/redundants_mlfs_red.csv",10)

analyzeAndPrint("distanceFequency_Redundant.pdf", ytitle="% of Mutant Pairs")

}


FEQUIVALENT=FALSE
if(FEQUIVALENT){
  

  
  analyzeAndPrint("distanceFequency_Equivalent_Reduced.pdf")
  
}




FREDUNDANT=FALSE
if(FREDUNDANT){
  
  evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/redundants_esail_red.csv",1)
  evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/redundants_csp_red.csv",2)
  evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/redundants_param_red.csv",3)
  evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/redundants_libutil_red.csv",4)
  evalHist("/Users/oscar.cornejo/University\ of\ Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/REDUCED-SMTS/redundants_mlfs_red.csv",5)
  
  analyzeAndPrint("distanceFequency_Duplicate_Reduced.pdf")
  
}


#evalHist( "/Users/oscar.cornejo/University of Luxembourg/P_SnT ESA_FAQAS - FAQAS_CodeDriven_TestSuiteAssessment/JournalData/RQ7_Updated/redundants_esail.csv" )
