#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>     /* assert */
#include "FAQAS_dataDrivenMutator.h"

int main()
{
    // Create a vector containing integers
    std::vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);


    std::vector<int> n;

    n=v;

    int position = _FAQAS_selectItem();


    //MANUALLY ADDED PROBE
    mutate_FM_IfHK( &v );
    //MANUALLY ADDED PROBE END

    if (position!= -999){
      assert( v.at(position) != n.at(position));
      assert( v.at(position) <= 5 );
      assert( v.at(position) >= -5 );
  }


    return 0;
}
