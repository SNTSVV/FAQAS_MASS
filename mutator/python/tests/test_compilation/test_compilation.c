#include "FAQAS_dataDrivenMutator.h"
#include <stdio.h>


//mutate function
int mutate(long long int *v , FaultModel *fm ){
    return _FAQAS_mutate(v,fm);
}
//end


int main(){
printf("Compiler test:\n");

printf("...\n");

printf("...\n");

long long int v[]={1, 2, 3, 4, 5, 6};

printf("OLD_VALUE\n");

printf("%llu\n",v[0]);

printf("...\n");

printf("...\n");

FaultModel *fm =  _FAQAS_General_FM();
mutate( v, fm );


printf("NEW_VALUE\n");

printf("%llu\n",v[0]);

printf("...\n");

printf("...\n");

#ifdef __cplusplus
  printf("c++\n");
#else
  printf("good old c\n");
#endif

return 0;
}
