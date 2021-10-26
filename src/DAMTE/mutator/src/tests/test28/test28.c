#include "FAQAS_dataDrivenMutator.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

int mutate(std::vector<double> *v, FaultModel *fm) {
  return _FAQAS_mutate(v->data(), fm);
}

int main() {
  int i;

  for (i = 0; i <= 10; i = i + 1) {
    // Create a vector containing doubles
    std::vector<double> v;

    v.push_back(i);
    v.push_back((double)i + 1);
    v.push_back((double)i + 2);
    v.push_back((double)i + 3);
    v.push_back((double)i + 4);

    printf("*********%f**********\n", v.at(1));

    // MANUALLY ADDED PROBE
    mutate_FM_IfHK( &v );
    // MANUALLY ADDED PROBE END

    printf("*********%f**********\n", v.at(1));

    for (std::vector<double>::iterator it = v.begin(); it != v.end(); ++it) {
      std::cout << *it << '\n';
    }
  }


  return 0;
}
