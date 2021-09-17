#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"


int main()
{
    // Create a vector containing integers
    std::vector<float> v;

    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);

  for (int i = 0; i < 10; i++){

    //MANUALLY ADDED PROBE
    mutate_FM_IfHK( &v );
    //MANUALLY ADDED PROBE END

  }



    for(std::vector<float>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }

    return 0;
}
