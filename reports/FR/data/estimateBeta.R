library(EnvStats)
library(robustbase)
library(MESS)

N=1000



COA_300 = FALSE
COA_400 = FALSE
COA_1000 = FALSE

#COMPUTE_OPTIMAL_R = FALSE
COMPUTE_OPTIMAL_R = TRUE

g<-function(Y,p,N){
  a<-(Y-N*p)
  res<-(a*a-(1-2*p)*(Y-N*p)-N*p*(1-p))/(2*p*(1-p))
  #res<-(a*a+Y*(2*p-1)-N*p*p)/(2*p*p*(1-p)*(1-p))
  return (res) 
}

cf<-function(Y,N,P,r){
  
  res <- dbinom(Y, size=N, prob=P)*(1+r*g(Y,P,N))
  return (res) 
}

evalDensity <- function(file,N,P,eP,eR,topText=NULL,FSCI=FALSE,suffix="_NLS"){
  
  data <- read.csv(file, header = TRUE, sep=";" )
  
  if ( FSCI == TRUE ){
    scores=data[,7]*10
  } else {
    scores=data[,7]/100*N
  }
  
  
  minD=min(scores)
  
  d<-density(scores, kernel = "gaussian", from=0, to=N, n=(N+1))
  
  
  yS <- unlist (d[2])
  
  
  fdest = paste0(file,suffix,"_density.pdf")
  
  pdf(file = fdest)
  
  legendNames=c("Samples","Binomial","Correlated Binomial (estimated parameters)","Correlated Binomial (r=0.0010)")
  selectedColors=c("black","red","blue","purple")
  
  topY=0.06
  
  plot(d,col=selectedColors[1], xlim=c(N/2, (N-(N/9) )) , ylim=c(0,topY), xlab="", ylab="", lty=2, main="", cex.lab=1.5, cex.axis=1.5)
  title(main = "", sub = "", xlab = "", ylab = "", line = NA, outer = FALSE )
  
  # Create a sample of 50 numbers which are incremented by 1.
  x <- seq(0,N,by = 1)
  
  # Create the binomial distribution.
  y <- dbinom(x,N,P)
  b = points(y, col=selectedColors[2], type="l" )
  
  yR <- sapply(x,cf,N=N,P=eP,r=eR)
  b = points(yR, col=selectedColors[3], type="l" )
  
  
  #plot(y,col="green")
  
  
  
  
  #yRR <- sapply(x,cf,N=N,P=P,r=0.0005)
  #b = points(yRR, col=selectedColors[4], type="l" )
  
  yRRR <- sapply(x,cf,N=N,P=P,r=0.0010)
  b = points(yRRR, col=selectedColors[4], type="l" )

  if ( ! is.null (topText) ){
    text((2*N/3), topY, labels=topText, cex=1.5 )
  }
  
  # legend(50,topY,legendNames,
  #        #col=selectedColors,
  #        lty=c(2,1,1,1,1),
  #        col=selectedColors,
  #        #fill=selectedColors,
  #        border=selectedColors,horiz=F)
  
  # if ( FALSE ){
  # x <- seq(0,30,by = 1)
  # yR <- sapply(x,cf,N=30,P=0.6,r=0.05)
  # plot(yR,col="red", ylim=c(0,0.15))
  # 
  # y <- dbinom(x,30,0.6)
  # b = points(y, col="green" )
  # }
  
  dev.off()
  
  print (typeof(y))
  print (typeof(yS))
  
  fitBin = y - yS
  fitEst = yR - yS
  fitBin[ fitBin < 0 ] <- 0
  fitEst[ fitEst < 0 ] <- 0
  
  
  
  aucSample <- auc ( x, y, type="spline" )
  
  aucFitBin <- auc ( x, fitBin, type="spline" )
  aucFitEst <- auc ( x, fitEst, type="spline" )
  
  fitBin = y - yS
  fitEst = yR - yS
  fitBin[ fitBin > 0 ] <- 0
  fitEst[ fitEst > 0 ] <- 0
  aucFitBinErr <- auc ( x, (-1 * fitBin), type="spline" )
  aucFitEstErr <- auc ( x, (-1 * fitEst), type="spline" )
  
  paucBin <- (1-(aucFitBin/(aucSample+aucFitBinErr)))*100
  paucEst <- (1-(aucFitEst/(aucSample+aucFitEstErr)))*100
  
  print("\n\nAUC Result for")
  print(file)
  print(N)
  print("AUC BIN")
  print(paucBin)
  print("AUC EST")
  print(paucEst)
  print("\n\n")
 
  
}

estimate <- function(file){
  
  data <- read.csv(file, header = TRUE, sep=";" )
  
  scores=data[,7]/100
  
 
  
  res<-ebeta(scores, method = "mle")
  
  shapes<-unlist(res[3])
  shape1<-shapes[1]
  shape2<-shapes[2]
  
  sm=mean(scores)
  print(sm)
  
  # Create a sample of 50 numbers which are incremented by 1.
  
  # Create the binomial distribution.
  
  
  # Give the chart file a name.
  pdf(file = "dbinom.pdf")
  
  # Plot the graph for this sample.
  #plot(x,y,col="green")
  
  # Save the file.
  
  
  x_beta <- seq(0, 1, by = 0.001)  
  print(x_beta)
  y_beta <- dbeta(x_beta, shape1 = 902, shape2 = 366)    # Apply beta function
  y <- dbinom(x_beta,1,sm)
  
  plot(x_beta,y_beta,col="red")
  
  
  b = points(x=x_beta, y, col="green" )
  
  dev.off()
  
  nlrob()
}


resetCoefficientOfAssociation<- function(runLen, cases){
  Q <<- matrix(data=NA,nrow=cases,ncol=runLen*runLen)
  R <<- matrix(data=NA,nrow=cases,ncol=runLen*runLen)
  Cov <<- matrix(data=NA,nrow=cases,ncol=runLen*runLen)
  Odds <<- matrix(data=NA,nrow=cases,ncol=runLen*runLen)
}

computeCoefficientOfAssociation <- function(file,runLen, exp){
  
  data <- data.matrix( read.csv(file, header = FALSE, sep=";" ) )
  
  
  
  
  print(data[1,1])
  
  
    
    
    for ( mutantI in 1:(runLen-1) ){
      
      s<-(mutantI+1)
      
      
      
      for ( mutantJ in s:runLen ){
        
      one_one = 0
      one_zero = 0
      zero_one = 0
      zero_zero = 0
     
      for ( simulation in 1:100 ){
        
        
        
        
        MUT_I <- data[mutantI,simulation]
        
        
        MUT_J <- data[mutantJ,simulation]
        
        
        if ( MUT_I == 1 ){
          if ( MUT_J == 1 ){
            one_one = one_one+1
          } else {
            one_zero = one_zero+1
          }
        } else {
          if ( MUT_J == 1 ){
            zero_one = zero_one+1
          } else {
            zero_zero = zero_zero+1
          }
        }
      }
    }
    
    a=one_one
    b=one_zero
    c=zero_one
    d=zero_zero
    
    
    
    P_J=(a+c)/(a+b+c+d)
    P_I=(a+b)/(a+b+c+d)
    
    E_Xi_Xj=(a/(a+b+c+d))
    
    E_Xi_Xj=(P_I*P_J)/(a+b+c+d)
    #-P_J*P_I

    covar <- (E_Xi_Xj-(P_I*P_J))
    covar <- cov(data[mutantI,],data[mutantJ,])
    
    r=covar/sqrt(P_I*P_J*(1-P_I)*(1-P_J))
    r<-cor(data[mutantI,],data[mutantJ,])
    
    pos = (mutantI-1)*runLen+mutantJ
    
    Q[exp,pos] <<- (a*d-b*c)/(a*d+b*c)
    
    ODDS <- (a*d)/(c*b)
    Odds[exp,pos] <<- ODDS
      
    R[exp,pos] <<- r
    Cov[exp,pos] <<- covar
  }
}

greedyFindR<-function(file,runLen,realP){
  data <- data.matrix( read.csv(file, header = FALSE, sep=";" ) )
  minESS <- 10000
  minP <- 0
  MS <- rep(NA,100)
  
  for ( simulation in 1:100 ){
    res<-0
    for ( mutant in 1:runLen ){
      res<-res+data[mutant,simulation]
    }
    MS[simulation]<-res
  }
  
  delta=0.005
  myP<-(realP-delta)

  minR<-0.0

  while ( myP <= (realP+delta) ){

    r<-+0.00000
    while ( r < 0.01 ){


      ESS<-0

      for ( Y in 1:runLen ){
        count=sum(MS == Y)
        ms=count/100
        p<-dbinom(Y,runLen,myP)

        ESS <- ESS + (ms-p*(1+r*g(Y,myP,runLen)))^2
      }


      if ( ESS < minESS ){
        minESS<-ESS
        minR <- r
        minP <- myP
      }

      r <- r+0.00001

    }
    myP<-myP+0.001
  }

  print("RunLen:")
  print(runLen)
  print("File:")
  print(file)
  print("r:")
  print(minR)
  print("p:")
  print(minP)
  print("ESS:")
  print(minESS)
  
  EP<<-minP
  ER<<-minR
  
  ESS<-0
  
  for ( Y in 1:runLen ){
    count=sum(MS == Y)
    ms=count/100
    p<-dbinom(Y,runLen,realP)
    r<-0
    ESS <- ESS + (ms-p*(1+r*g(Y,realP,runLen)))^2
  }
  
  print("Reference binomial")
  print("RunLen:")
  print(runLen)
  print("File:")
  print(file)
  print("r:")
  print(r)
  print("p:")
  print(realP)
  print("ESS:")
  print(ESS)
}

rf<-function(x,runLen,myP,r){
  p<-dbinom(x,runLen,myP)
  return ( p*(1+r*g(x,myP,runLen)) )
}


findR<-function(file,runLen,realP){
  data <- data.matrix( read.csv(file, header = FALSE, sep=";" ) )
  
  MS <- rep(NA,100)
  
  for ( simulation in 1:100 ){
    res<-0
    for ( mutant in 1:runLen ){
      res<-res+data[mutant,simulation]
    }
    MS[simulation]<-res
  }
  
  YS <- rep(NA,runLen)    
      for ( Y in 1:runLen ){
        count=sum(MS == Y)
        ms=count/100
        
        
        YS[Y]<-ms
        
        
      }
  
  x <- c(1:runLen)    
  m <- nls(YS~rf(x,runLen,myP,r),  
           start = list(myP = realP, r = 0.0)) 
  
  #print(m)
  #print(summary(m))
  minP <- unlist(coef(m))[1]
  minR <- unlist(coef(m))[2]
  #print(resid(m))
  
  
  
  # print minimum residual or error value 
  ESS<-(sum(resid(m)^2)) 
  
  EP<<-minP
  ER<<-minR
  
  print("RunLen:")
  print(runLen)
  print("File:")
  print(file)
  print("r:")
  print(minR)
  print("p:")
  print(minP)
  print("ESS:")
  print(ESS)
  # 
  # 
}


plotCoefficientMatrix <- function(Q,file, rangeYa=-1, rangeYb=1){
  pdf(file = file)
  
  qb <- boxplot(t(Q),use.cols=TRUE,xlab="",
                ylab="Q",
                ylim=c(rangeYa,rangeYb),
                na.rm=TRUE, xaxt="n")
  
  mtext(
    c("ESAIL","LIBN","LIBP","LIBU","MLFS"), side = 1, line = 1, outer = FALSE, at = c(1:5))
  
  print(qb)
  
  dev.off()
}

plotCoefficientOfAssociation <- function(file,fileR, fileC, fileO){
  
  
  plotCoefficientMatrix(Q, file)
  
  plotCoefficientMatrix(R, fileR)
  
  plotCoefficientMatrix(Cov, fileC)
  
  plotCoefficientMatrix(Odds, fileO, rangeYa=0,rangeYb=5)
  
  
}
#estimate("mutationScores/MS_1000_libutil.csv")






if ( COA_300 ){					
  resetCoefficientOfAssociation(300,5)
  
  computeCoefficientOfAssociation("mutationResultsAll/esail_cochran.csv",300,1)
  computeCoefficientOfAssociation("mutationResultsAll/csp_cochran.csv",300,2)
  computeCoefficientOfAssociation("mutationResultsAll/param_cochran.csv",300,3)
  computeCoefficientOfAssociation("mutationResultsAll/libutil_cochran.csv",300,4)
  computeCoefficientOfAssociation("mutationResultsAll/mlfs_cochran.csv",300,5)
  
  plotCoefficientOfAssociation("Q300.pdf","R300.pdf","Cov300.pdf","Odds300.pdf")
}

if ( COA_400 ){	
  nMutants=400
  resetCoefficientOfAssociation(nMutants,5)
  
  computeCoefficientOfAssociation("mutationResultsAll/esail_cochran.csv",nMutants,1)
  computeCoefficientOfAssociation("mutationResultsAll/csp_cochran.csv",nMutants,2)
  computeCoefficientOfAssociation("mutationResultsAll/param_cochran.csv",nMutants,3)
  computeCoefficientOfAssociation("mutationResultsAll/libutil_cochran.csv",nMutants,4)
  computeCoefficientOfAssociation("mutationResultsAll/mlfs_cochran.csv",nMutants,5)
  
  plotCoefficientOfAssociation("Q400.pdf","R400.pdf","Cov400.pdf","Odds400.pdf")
}

if ( COA_1000 ){	
  nMutants=1000
  resetCoefficientOfAssociation(nMutants,5)
  
  computeCoefficientOfAssociation("mutationResultsAll/esail_cochran.csv",nMutants,1)
  computeCoefficientOfAssociation("mutationResultsAll/csp_cochran.csv",nMutants,2)
  computeCoefficientOfAssociation("mutationResultsAll/param_cochran.csv",nMutants,3)
  computeCoefficientOfAssociation("mutationResultsAll/libutil_cochran.csv",nMutants,4)
  computeCoefficientOfAssociation("mutationResultsAll/mlfs_cochran.csv",nMutants,5)
  
  plotCoefficientOfAssociation("Q1000.pdf","R1000.pdf","Cov1000.pdf","Odds1000.pdf")
}


fdest = paste0("density_LEGEND.pdf")
pdf(fdest,width = 7, height = 4.9)
plot(NULL ,xaxt='n',yaxt='n',bty='n',ylab='',xlab='', xlim=c(0,2), ylim=c(0,0.05))

legendNames=c("PMF of Samples","PMF of Binomial","PMF of Correlated Binomial \n(with estimated parameters)","PMF of Correlated Binomial (r=0.0010)")
selectedColors=c("black","red","blue","purple")

legend(0,0.05, legend =legendNames, 
       #pch=16, 
       pt.cex=3, cex=1.5, 
       lty=c(2,1,1,1),
       col=selectedColors,
       #fill=selectedColors,
       border=selectedColors,
       horiz=F)

#mtext("Legend", at=0.2, cex=2)
dev.off()



if ( COMPUTE_OPTIMAL_R ){					
  findR("mutationResultsAll/esail_cochran.csv",300,65.36/100)
  evalDensity("mutationScores/MS_300_esail.csv",300,65.36/100,EP,ER,topText="(a) ESAIL (N=300)")
  
  findR("mutationResultsAll/csp_cochran.csv",300,65.64/100)
  evalDensity("mutationScores/MS_300_csp.csv",300,65.64/100,EP,ER,topText="(b) LIBN (N=300)")
  
  findR("mutationResultsAll/param_cochran.csv",300,69.12/100)
  evalDensity("mutationScores/MS_300_param.csv",300,69.12/100,EP,ER,topText="(c) LIBP (N=300)")
  
  findR("mutationResultsAll/libutil_cochran.csv",300,71.20/100)
  evalDensity("mutationScores/MS_300_libutil.csv",300,71.20/100,EP,ER,topText="(d) LIBU (N=300)")
  
  findR("mutationResultsAll/mlfs_cochran.csv",300,81.80/100)
  evalDensity("mutationScores/MS_300_mlfs.csv",300,81.80/100,EP,ER,topText="(e) MLFS (N=300)")

  
  
  topText="ESAIL (N=300)"
  topText="LIBN (N=300)"
  topText="LIBP (N=300)"
  topText="LIBU (N=300)"
  topText="MLFS (N=300)"
  
  
  
  findR("mutationResultsAll/esail_cochran.csv",400,65.36/100)
  evalDensity("mutationScores/MS_400_esail.csv",400,65.36/100,EP,ER,topText="(f) ESAIL (N=400)")

  findR("mutationResultsAll/csp_cochran.csv",400,65.64/100)
  evalDensity("mutationScores/MS_400_csp.csv",400,65.64/100,EP,ER, topText="(g) LIBN (N=400)")
  
  findR("mutationResultsAll/param_cochran.csv",400,69.12/100)
  evalDensity("mutationScores/MS_400_param.csv",400,69.12/100,EP,ER,topText="(h) LIBP (N=400)")
  
  findR("mutationResultsAll/libutil_cochran.csv",400,71.20/100)
  evalDensity("mutationScores/MS_400_libutil.csv",400,71.20/100,EP,ER,topText="(i) LIBU (N=400)")
  
  findR("mutationResultsAll/mlfs_cochran.csv",400,81.80/100)
  evalDensity("mutationScores/MS_400_mlfs.csv",400,81.80/100,EP,ER,topText="(l) MLFS (N=400)")
  
  
  
  
  
  findR("mutationResultsAll/esail_cochran.csv",1000,65.36/100)
  evalDensity("mutationScores/MS_1000_esail.csv",1000,65.36/100,EP,ER, topText="(m) ESAIL (N=1000)")
  
  findR("mutationResultsAll/csp_cochran.csv",1000,65.64/100)
  evalDensity("mutationScores/MS_1000_csp.csv",1000,65.64/100,EP,ER, topText="(n) LIBN (N=1000)")
  
  findR("mutationResultsAll/param_cochran.csv",1000,69.12/100)
  evalDensity("mutationScores/MS_1000_param.csv",1000,69.12/100,EP,ER, topText="(o) LIBP (N=1000)")
  
  findR("mutationResultsAll/libutil_cochran.csv",1000,71.20/100)
  evalDensity("mutationScores/MS_1000_libutil.csv",1000,71.20/100,EP,ER, topText="(p) LIBU (N=1000)")
  
  findR("mutationResultsAll/mlfs_cochran.csv",1000,81.80/100)
  evalDensity("mutationScores/MS_1000_mlfs.csv",1000,81.80/100,EP,ER, topText="(q) MLFS (N=1000)")
}