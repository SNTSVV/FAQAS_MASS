#include "FAQAS_dataDrivenMutator.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

int mutate(std::vector<long int> *v, FaultModel *fm) {
  return _FAQAS_mutate(v->data(), fm);
}

int main() {
  int i;

  for (i = 0; i <= 10; i = i + 1) {
    // Create a vector containing long ints
    std::vector<long int> v;

    v.push_back((long int)i);
    v.push_back((long int)i + 1);
    v.push_back((long int)i + 2);
    v.push_back((long int)i + 3);
    v.push_back((long int)i + 4);

    printf("*********%ld**********\n", v.at(1));

    // MANUALLY ADDED PROBE
    mutate_FM_IfHK( &v );
    // MANUALLY ADDED PROBE END

    printf("*********%ld**********\n", v.at(1));

    for (std::vector<long int>::iterator it = v.begin(); it != v.end(); ++it) {
      std::cout << *it << '\n';
    }
  }



  return 0;
}
