#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"


int mutate( std::vector<int> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}


int main()
{


  int counter;

  for (counter=0; counter <= 10; counter=counter+1){

    // Create a vector containing integers
    std::vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);


    //MANUALLY ADDED PROBE




      FaultModel *fm = _FAQAS_IfHK_FM();
      mutate( &v, fm );

      for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it){

        std::cout << *it << '\n';

      }

    }


    //MANUALLY ADDED PROBE END



    return 0;
}
