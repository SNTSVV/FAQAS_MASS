#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>     /* assert */
#include <stdio.h>
#include "FAQAS_dataDrivenMutator.h"

//this test will be used for INV operators

int mutate( std::vector<long long int> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}

void push_back(std::vector<long long int> *v, double val){

  long long int tmp = 0;

	memcpy ( &tmp, &val, sizeof(double) );

	v->push_back(tmp);
}

int main()
{
    // Create a vector containing integers
    std::vector< long long int> v;

    push_back(&v,1.1);
    push_back(&v,2.2);
    push_back(&v,3.3);
    push_back(&v,4.4);
    push_back(&v,5.5);


    std::vector<long long int> n;



    int position = _FAQAS_selectItem();

    //MANUALLY ADDED PROBE
    
    

    //MANUALLY ADDED PROBE END



    for(std::vector<long long int>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }


    return 0;
}
