#include "FAQAS_dataDrivenMutator.h"
#include <assert.h> /* assert */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// this test will be used for INV operators
void print_float(int val) {
  float tmp = 0;

  memcpy(&tmp, &val, sizeof(int));

  std::cout << tmp << '\n';
}

float as_float(int val) {
  float tmp = 0;

  memcpy(&tmp, &val, sizeof(int));

  return tmp;
}

void push_back(std::vector<int> *v, float val) {
  long int tmp = 0;

  memcpy(&tmp, &val, sizeof(float));

  v->push_back(tmp);
}

int main() {
  // Create a vector containing doubles
  std::vector<int> v;

  push_back(&v, 1.1);
  push_back(&v, 2.2);
  push_back(&v, 3.3);
  push_back(&v, 4.4);
  push_back(&v, 5.5);

  std::vector<int> n;

  n = v;

  int position = _FAQAS_selectItem();

  for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    // std::cout << *it << '\n';
    print_float(*it);
  }

  std::cout << "Mutation\n";

  // MANUALLY ADDED PROBE
  mutate_FM_IfHK( &v );
  // MANUALLY ADDED PROBE END

  for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {

    print_float(*it);
  }

  return 0;
}
