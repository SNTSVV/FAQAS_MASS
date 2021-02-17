#include <iostream>
#include <vector>
#include <stdlib.h>
#include <assert.h>     /* assert */
#include <stdio.h>
#include "FAQAS_dataDrivenMutator.h"

//this test will be used for INV operators

int mutate( std::vector<int> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}

void print_float(int val){
	float tmp = 0;


     	//std::cout << "long \n";
     	//std::cout << val << '\n';

	memcpy ( &tmp, &val, sizeof(int) );

     	//std::cout << "float \n";
     	std::cout << tmp << '\n';
}

float as_float(int val){
	float tmp = 0;


	memcpy ( &tmp, &val, sizeof(int) );

	return tmp;
}

void push_back(std::vector<int> *v, float val){
	long int tmp = 0;

     	//std::cout << "PB float \n";
     	//std::cout << val << '\n';

	memcpy ( &tmp, &val, sizeof(float) );

     	//std::cout << "PB long \n";
     	//std::cout << tmp << '\n';

     	//std::cout << "PB long as float \n";
	//print_float(tmp);

	v->push_back(tmp);
}


int main()
{
    // Create a vector containing integers
    std::vector<int> v;

    push_back(&v,1.1);
    push_back(&v,2.2);
    push_back(&v,3.3);
    push_back(&v,4.4);
    push_back(&v,5.5);


    std::vector<int> n;

    n=v;

    int position = _FAQAS_selectItem();


    for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
     	//std::cout << *it << '\n';
	print_float(*it);
    }

    std::cout << "Mutation\n";

    //MANUALLY ADDED PROBE
    FaultModel *fm = _FAQAS_IfHK_FM();
    mutate( &v, fm );
    //MANUALLY ADDED PROBE END


    for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    	//std::cout << *it << '\n';
	print_float(*it);
    }

  //   if (position!= -999){
	// for( int j=0; j<5; j++ ){
	// 	float a = as_float ( v.at(j) );
	// 	float b = as_float ( n.at(j) );
	// 	assert( faqas_float_equal(a,b) );
	// }
  //   }

    return 0;
}
