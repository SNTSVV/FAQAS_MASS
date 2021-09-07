#include "FAQAS_dataDrivenMutator.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

int main() {
  // Create a vector containing hexadecimal numbers
  std::vector<int> v;

  v.push_back(0x15); //== (Integer) 21
  v.push_back(0x01); // 1
  v.push_back(0x06); // 6
  v.push_back(0x06); // 6
  v.push_back(0x19); // 25

  // MANUALLY ADDED PROBE
  mutate_FM_IfHK( &v );
  // MANUALLY ADDED PROBE END

  for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << '\n';
  }

  return 0;
}
