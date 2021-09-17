#include "FAQAS_dataDrivenMutator.h"
#include <iostream>
#include <stdlib.h>
#include <vector>


int main() {
  // Create a vector containing integers
  std::vector<int> v;

  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);

  // MANUALLY ADDED PROBE
  mutate_FM_IfHK( &v );
  // MANUALLY ADDED PROBE END

  for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << '\n';
  }

  return 0;
}
