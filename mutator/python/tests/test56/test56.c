#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"


int mutate( std::vector<signed char> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}


int main()
{

    // Create a vector containing integers
    std::vector<signed char> v;
    v.push_back(64);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    v.push_back(75);
    
    //MANUALLY ADDED PROBE
    FaultModel *fm = _FAQAS_IfHK_FM();
    mutate( &v, fm );
    _FAQAS_delete_FM(fm);
    //MANUALLY ADDED PROBE END

    for(std::vector<signed char>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }

    return 0;
}
