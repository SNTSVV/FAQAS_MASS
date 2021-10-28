#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"

int main()
{

    // Create a vector containing integers
    std::vector<long int> v;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);
    v.push_back(50);
    v.push_back(60);
    v.push_back(70);
    v.push_back(80);
    v.push_back(90);
    v.push_back(100);
    v.push_back(110);
    v.push_back(120);
    v.push_back(130);
    v.push_back(140);
    v.push_back(150);
    v.push_back(160);
    v.push_back(170);
    v.push_back(180);
    v.push_back(190);
    v.push_back(200);


    //MANUALLY ADDED PROBE
    mutate_FM_IfHK( &v );
    //MANUALLY ADDED PROBE END

    for(std::vector<long int>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }

    return 0;
}
