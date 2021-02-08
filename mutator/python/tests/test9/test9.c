#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"


int mutate( std::vector<float> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}


int main()
{
    // Create a vector containing integers
    std::vector<float> v;

    v.push_back(1.1);
    v.push_back(2.2);
    v.push_back(3.3);
    v.push_back(4.4);
    v.push_back(5.5);


    std::vector<float> n;

    n=v;

    int position = _FAQAS_selectItem();

    //MANUALLY ADDED PROBE
    FaultModel *fm = _FAQAS_IfHK_FM();
    mutate( &v, fm );
    //MANUALLY ADDED PROBE END

    if (position!= -999){
      assert( v.at(position) != n.at(position));
      assert( v.at(position) <= 5.5 );
      assert( v.at(position) >= -5.5 );
  }
    return 0;
}
