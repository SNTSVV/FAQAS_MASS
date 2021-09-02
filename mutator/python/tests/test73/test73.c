#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"


int main()
{
    // Create a vector containing integers
    std::vector<unsigned int> v;

    v.push_back(1);
    v.push_back(3);
    v.push_back(8);

    //MANUALLY ADDED PROBE
    mutate_FM_IfHK( &v );
    //MANUALLY ADDED PROBE END


    for(std::vector<unsigned int>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }

    return 0;
}
