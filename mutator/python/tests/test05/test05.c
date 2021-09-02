#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>     /* assert */
#include "FAQAS_dataDrivenMutator.h"

//this test will be used for INV operators
int main()
{
    // Create a vector containing doubles
    std::vector<double> v;

    v.push_back(1.1);
    v.push_back(2.2);
    v.push_back(3.3);
    v.push_back(4.4);
    v.push_back(5.5);


    std::vector<double> n;

    n=v;

    int position = _FAQAS_selectItem();


    //MANUALLY ADDED PROBE
    mutate_FM_IfHK( &v );
    //MANUALLY ADDED PROBE END


    if (position!= -999){
      assert( v.at(position) != n.at(position));
      assert( v.at(position) <= 5.5 );
      assert( v.at(position) >= -5.5 );
  }


    return 0;
}
