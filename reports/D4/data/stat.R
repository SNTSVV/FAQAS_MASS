library(plyr)
library(effsize)

computeVDA <- function(title,APPR,BL,BLname){
  
  APPR = na.omit( APPR )
  BL = na.omit( BL )
  
  
  
  vd=VD.A(d = APPR,f = BL, na.action="na.omit" )
  
  print(vd)
  
  ARES=unlist(vd)
  aEstimate=as.numeric(ARES[4])
  
  if ( is.na(aEstimate) ){
    print(nrow(APPR))
    print(ncol(APPR))
    print(APPR)
    print(nrow(BL))
    print(ncol(BL))
    print(BL)
    line=paste0(title," VD-A: CANNOT COMPUTE for APPR vs ",BLname,"\n")
  } else {
    
    if(aEstimate>=0.56){
      line=paste0(title," VD-A: APPR BETTER than ",BLname,"\n")
      #VSignificanceMatrix[Srow,Scol]<<-1
      if(aEstimate>=0.71){
        line=paste0(line," (large)")
      } else if(aEstimate>0.64){
        line=paste0(line," (medium)")
      } else {
        line=paste0(line," (small)")
      }
    } else {
      if(aEstimate>=0.5){
        line=paste0(title," VD-A: (negligible) APPR BETTER than ",BLname,"\n")
      }
    }
    
    if(aEstimate<=0.46){
      line=paste0(title," VD-A: ",BLname,"BETTER than APPR \n")
      #VSignificanceMatrix[Srow,Scol] <<- -1
      
      if(aEstimate<=0.29){
        line=paste0(line," (large)")
      } else if(aEstimate<=0.36){
        line=paste0(line," (medium)")
      } else {
        line=paste0(line," (small)")
      }
    } else {
      if(aEstimate<0.5){
        line=paste0(title," VD-A: ",BLname,"BETTER than APPR (negligible) \n")
      }
    }
    
    if(aEstimate==0.5){
      line=paste0(title," VD-A: APPR SAME as ",BLname,"\n")
    }
  }
  
  line=paste0(line,"\n(",format(aEstimate, nsmall=5),")")
  
  return(line)
}

statisticalSignificance<-function(title,APPR,BL,BLname){
  
  #print("LEN")
  L=length(APPR)
  #print(L)
  
  
  
  line=paste0(L," ELEMENTS. ",BLname,"\n")
  #write(line,file=STATISTICAL_SIGNIFICANCE_FILE,append=TRUE)
  
  
  notAvailable = sum(is.na(APPR))
  
  notAvailableBL = sum(is.na(BL))
  
  available = (L-notAvailable)
  availableBL = (L-notAvailableBL)
  if ( available <= 1 || availableBL <= 1){
    line=paste0(title," DATA NOT AVAILABLE ",BLname,"\n")
  } else {

    
    w=wilcox.test(APPR,BL,alternative = "two.sided", exact = FALSE, correct = FALSE, na.action="na.omit" )
    print(w)
    
    U=unlist(w)
    W=as.numeric(U[1])
    P=as.numeric(U[2])
    
    
    
    print (P)
    if ( ! is.na(P) ){
    if ( P < 0.05 ){
      line=paste0(title," WILCOX: DIFFERNCE IS SIGNIFICANT ",BLname,"\n")
      
      #WSignificanceMatrix[Srow,Scol]<<-1
    } else {
      line=paste0(title," WILCOX: DIFFERNCE IS NOT SIGNIFICANT ",BLname,"!!!! \n")
    }
      
      
      
      line=paste0(line,"\n P-value: ",format(P, nsmall=5),"\n")
    }
  }
  
  
  print(line)
  
  
 
  
  line=computeVDA(title,APPR,BL,BLname)
  print(line)
  
  
  
  #w=wilcox.test(HUDD,BL2)
}

APPR<-c(53.9,29.8,10.1,5.5,0.5,0.2)
BL<-c(25.3,47.7,18.0,8.4,0.6,0.0)

APPR<-c(0.35,0.85,1.0,1.0,1.0,1.0)
BL<-c(0.35,0.95,1.0,1.0,1.0,NA)


statisticalSignificance("ALL",APPR,BL,"SMTS")