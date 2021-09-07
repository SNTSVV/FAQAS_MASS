#include "FAQAS_dataDrivenMutator.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

int main() {
  // Create a vector containing hexadecimal numbers
  std::vector<long int> v;

  v.push_back(0x05); //== (Integer) 21
  v.push_back(0x05); // 22
  v.push_back(0x05); // 23
  v.push_back(0x05); // 24
  v.push_back(0x05); // 25

  // MANUALLY ADDED PROBE
  mutate_FM_IfHK( &v );
  // MANUALLY ADDED PROBE END

  for (std::vector<long int>::iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << '\n';
  }

  return 0;
}
