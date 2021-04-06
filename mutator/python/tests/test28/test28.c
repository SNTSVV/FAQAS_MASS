#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"


int mutate( std::vector<double> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}


int main()
{
  int i;

      FaultModel *fm = _FAQAS_IfHK_FM();

    for (i=0; i<=10; i=i+1){
      // Create a vector containing integers
    std::vector<double> v;

    v.push_back(i);
    v.push_back(i+1);
    v.push_back(i+2);
    v.push_back(i+3);
    v.push_back(i+4);

    printf("*********%d**********\n",v.at(1));

    //MANUALLY ADDED PROBE

    mutate( &v, fm );
_FAQAS_delete_FM(fm);
    //MANUALLY ADDED PROBE END
    printf("*********%d**********\n",v.at(1));


    for(std::vector<double>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }

    }

    return 0;
}
