#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"


int mutate( std::vector<long int> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}


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

    std::vector<long int> b;

    //MANUALLY ADDED PROBE
    FaultModel *fm = _FAQAS_one_FM();
    mutate( &v, fm );
    _FAQAS_delete_FM(fm);
    //MANUALLY ADDED PROBE END

    printf("vector 1\n");

    for(std::vector<long int>::iterator it = v.begin(); it != v.end(); ++it) {
      std::cout << *it << '\n';
    }

    b.push_back(110);
    b.push_back(120);
    b.push_back(130);
    b.push_back(140);
    b.push_back(150);
    b.push_back(160);
    b.push_back(170);
    b.push_back(180);
    b.push_back(190);
    b.push_back(200);


    //MANUALLY ADDED PROBE
    fm = _FAQAS_two_FM();
    mutate( &b, fm );
    _FAQAS_delete_FM(fm);
    //MANUALLY ADDED PROBE END

    printf("vector 2\n");

    for(std::vector<long int>::iterator it = b.begin(); it != b.end(); ++it) {
    	std::cout << *it << '\n';
    }

    

    return 0;
}
