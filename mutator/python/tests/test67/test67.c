#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"


int mutate( std::vector<double> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}


int main()
{
    // Create a vector containing integers
    std::vector<double> v;

    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);

    //MANUALLY ADDED PROBE
    FaultModel *fm = _FAQAS_IfHK_FM();
    mutate( &v, fm );
    _FAQAS_delete_FM(fm);
    //MANUALLY ADDED PROBE END

    for(std::vector<double>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }

    return 0;
}
