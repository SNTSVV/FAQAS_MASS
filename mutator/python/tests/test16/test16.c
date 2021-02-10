#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>     /* assert */
#include <stdio.h>
#include "FAQAS_dataDrivenMutator.h"

//this test will be used for INV operators

int mutate( std::vector<long int> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}

void push_back(std::vector<long int> *v, double val){
	long int tmp = 0;

	memcpy ( &tmp, &val, sizeof(double) );

	v->push_back(tmp);
}

int main()
{
    // Create a vector containing integers
    std::vector<long int> v;

    push_back(&v,1.1);
    push_back(&v,2.2);
    push_back(&v,3.3);
    push_back(&v,4.4);
    push_back(&v,5.5);


    std::vector<long int> n;

    n=v;

    int position = _FAQAS_selectItem();


    //MANUALLY ADDED PROBE
    FaultModel *fm = _FAQAS_IfHK_FM();
    mutate( &v, fm );
    //MANUALLY ADDED PROBE END



     // printf("********************************************\n");
     // printf("this is the position:%i\n", position);
     // printf("********************************************\n");
    // printf("this is v(1):%i\n", v.at(1));
    // printf("********************************************\n");


    for(std::vector<long int>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }


    return 0;
}
