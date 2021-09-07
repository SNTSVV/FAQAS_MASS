#include <iostream>
#include <vector>
#include <stdlib.h>
#include "DAMAt_dataDrivenMutator.h"


int main()
{
    // Create a vector containing integers
    std::vector<long int> v;

    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);

    //MANUALLY ADDED PROBE
    mutate_FM_IfHK( &v );
    //MANUALLY ADDED PROBE END

    for(std::vector<long int>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }

    return 0;
}
