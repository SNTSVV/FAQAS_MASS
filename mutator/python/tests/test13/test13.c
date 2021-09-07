#include "DAMAt_dataDrivenMutator.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

int main() {
  int i;

  for (i = 0; i <= 3; i = i + 1) {
    // Create a vector containing integers
    std::vector<double> v;

    v.push_back(i);
    v.push_back(i + 1);
    v.push_back(i + 2);
    v.push_back(i + 3);
    v.push_back(i + 4);

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
