#include <iostream>
#include <vector>
#include <stdlib.h>
#include "DAMAt_dataDrivenMutator.h"


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
    mutate_FM_IfHK( &v );
    //MANUALLY ADDED PROBE END

    for(std::vector<signed char>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }

    return 0;
}
