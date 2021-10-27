#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>     /* assert */

#include "FAQAS_dataDrivenMutator.h"

int main()
{
    // Create a vector containing integers
    std::vector<double> v;

    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);

    std::vector<double> n;

    n.push_back(0);
    n.push_back(0);
    n.push_back(0);
    n.push_back(0);
    n.push_back(0);

    //MANUALLY ADDED PROBE
    mutate_FM_IfHK( &v );
    //MANUALLY ADDED PROBE END
    int position = _FAQAS_selectItem();


    if (position!= -999){
      assert( v.at(position) != n.at(position));
  }

    return 0;
}
