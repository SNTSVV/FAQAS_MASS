#include <iostream>
#include <vector>
#include <stdlib.h>
#include "DAMAt_dataDrivenMutator.h"


int mutate( std::vector<long int> *v, FaultModel *fm ){
    return _DAMAt_mutate(v->data(),fm);
}


int main()
{

  // Create a vector containing integers

  std::vector<long int> v;
  std::vector<long int> b;
  std::vector<long int> p;


    b.push_back(100);
    b.push_back(100);
    b.push_back(100);
    b.push_back(100);
    b.push_back(100);
    b.push_back(100);
    b.push_back(100);
    b.push_back(100);
    b.push_back(100);
    b.push_back(100);

    v.push_back(100);
    v.push_back(100);
    v.push_back(100);
    v.push_back(100);
    v.push_back(100);
    v.push_back(100);
    v.push_back(100);
    v.push_back(100);
    v.push_back(100);
    v.push_back(100);

    p.push_back(100);
    p.push_back(100);
    p.push_back(100);
    p.push_back(100);
    p.push_back(100);
    p.push_back(100);
    p.push_back(100);
    p.push_back(100);
    p.push_back(100);
    p.push_back(100);

    //MANUALLY ADDED PROBE
    FaultModel *fm = _DAMAt_one_FM();
    mutate( &v, fm );
    _DAMAt_delete_FM(fm);
    //MANUALLY ADDED PROBE END

    //MANUALLY ADDED PROBE
    fm = _DAMAt_two_FM();
    mutate( &b, fm );
    _DAMAt_delete_FM(fm);
    //MANUALLY ADDED PROBE END

    //MANUALLY ADDED PROBE
    fm = _DAMAt_three_FM();
    mutate( &p, fm );
    _DAMAt_delete_FM(fm);
    //MANUALLY ADDED PROBE END


    printf("vector 1\n");

    for(std::vector<long int>::iterator it = v.begin(); it != v.end(); ++it) {
      std::cout << *it << '\n';
    }


    printf("vector 2\n");

    for(std::vector<long int>::iterator it = b.begin(); it != b.end(); ++it) {
    	std::cout << *it << '\n';
    }


    printf("vector 3\n");

    for(std::vector<long int>::iterator it = p.begin(); it != p.end(); ++it) {
      std::cout << *it << '\n';
    }


    return 0;
}
