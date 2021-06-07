#include <stdio.h>
#include <math.h>
#include <assert.h>

unsigned long long int FAQAS_simple_pow_without_math_library(int base, int power){

  if(power < 0){
    return 0;
  }

  unsigned long long int result=1;

  if(power >= 0){
    int step;
    for ( step=1; step <= power; step=step+1 ) {
      result= result * base;
    }
  }
  return result;
}

int main() {

   int esponenzio;
   for (esponenzio=0; esponenzio<=100; esponenzio=esponenzio+1){
     unsigned long long int a = FAQAS_simple_pow_without_math_library(2, esponenzio);
     unsigned long long int b = pow(2, esponenzio);
     printf("origginale: 2 alla %d = %llu\n", esponenzio, b);
     printf("tarocco: 2 alla %d = %llu\n", esponenzio, a);
     assert( a == b);

   }


   return 0;
}
