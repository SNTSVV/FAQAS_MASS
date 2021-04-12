#include "FAQAS_dataDrivenMutator.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

int mutate(std::vector<double> *v, FaultModel *fm) {
  return _FAQAS_mutate(v->data(), fm);
}

int main() {
  // Create a vector containing doubles
  std::vector<double> v;

  v.push_back(1.1);
  v.push_back(2.2);
  v.push_back(3.3);
  v.push_back(4.4);
  v.push_back(5.5);

  // MANUALLY ADDED PROBE
  FaultModel *fm = _FAQAS_IfHK_FM();
  mutate(&v, fm);
  _FAQAS_delete_FM(fm);
  // MANUALLY ADDED PROBE END

  for (std::vector<double>::iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << '\n';
  }

  return 0;
}
