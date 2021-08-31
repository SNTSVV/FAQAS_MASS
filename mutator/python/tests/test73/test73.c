#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"


int mutate( std::vector<unsigned int> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}


int main()
{
    // Create a vector containing integers
    std::vector<unsigned int> v;

    v.push_back(1);
    v.push_back(3);
    v.push_back(8);

    //MANUALLY ADDED PROBE
    FaultModel *fm = _FAQAS_IfHK_FM();
    mutate( &v, fm );
    _FAQAS_delete_FM(fm);
    //MANUALLY ADDED PROBE END

  
    for(std::vector<unsigned int>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }

    return 0;
}
