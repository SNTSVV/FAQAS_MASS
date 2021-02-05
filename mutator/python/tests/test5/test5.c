#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>     /* assert */
#include "FAQAS_dataDrivenMutator.h"

//this test will be used for INV operators

int mutate( std::vector<int> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}

//nuovo test 5
int main()
{
    // Create a vector containing integers
    std::vector<int> v;

    v.push_back(1.1);
    v.push_back(2.2);
    v.push_back(3.3);
    v.push_back(4.4);
    v.push_back(5.5);


    std::vector<int> n;

    n=v;

    int position = _FAQAS_selectItem();


    //MANUALLY ADDED PROBE
    FaultModel *fm = _FAQAS_IfHK_FM();
    mutate( &v, fm );
    //MANUALLY ADDED PROBE END



     // printf("********************************************\n");
     // printf("this is the position:%i\n", position);
     // printf("********************************************\n");
    // printf("this is v(1):%i\n", v.at(1));
    // printf("********************************************\n");


    if (position!= -999){
      assert( v.at(position) != n.at(position));
      assert( v.at(position) <= 5.5 );
      assert( v.at(position) >= -5.5 );
  }


    return 0;
}
