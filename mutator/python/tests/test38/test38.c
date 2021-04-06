#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"


int mutate( std::vector<int> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}


int main()
{
    // Create a vector containing hexadecimal numbers
    std::vector<int> v;

    v.push_back(0x15);//== (Integer) 21
    v.push_back(0x16);//22
    v.push_back(0x17);//23
    v.push_back(0x18);//24
    v.push_back(0x19);//25


    //MANUALLY ADDED PROBE
    FaultModel *fm = _FAQAS_IfHK_FM();
    mutate( &v, fm );
_FAQAS_delete_FM(fm);
    //MANUALLY ADDED PROBE END

    for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }

    return 0;
}
