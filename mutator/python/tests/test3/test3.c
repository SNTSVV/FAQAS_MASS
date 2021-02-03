#include <iostream>
#include <vector>
#include <stdlib.h> 
#include <assert.h>     /* assert */
#include "FAQAS_dataDrivenMutator.h"


int mutate( std::vector<int> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}

 
int main()
{
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
    //MANUALLY ADDED PROBE END


    data = v->data();

    assert( data.at(2) != 2 )
    assert( data.at(2) < 10 )
    assert( data.at(2) > 0 )

    return 0;
}
