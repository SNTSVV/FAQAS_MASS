#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>     /* assert */

#include "FAQAS_dataDrivenMutator.h"

int main()
{
    // Create a vector containing integers
    std::vector<int> v;

    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);

    std::vector<int> n;

    n.push_back(0);
    n.push_back(0);
    n.push_back(0);
    n.push_back(0);
    n.push_back(0);

    //MANUALLY ADDED PROBE
    mutate_FM_IfHK( &v );
    //MANUALLY ADDED PROBE END

    // for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    // 	std::cout << *it << '\n';
    // }
    int position = _FAQAS_selectItem();


    if (position!= -999){
      assert( v.at(position) != n.at(position));
  }

    return 0;
}
