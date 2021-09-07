#include "FAQAS_dataDrivenMutator.h"
#include <assert.h> /* assert */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// this test will be used for INV operators

void print_double(long long int val) {
  double tmp = 0;

  memcpy(&tmp, &val, sizeof(long long int));

  std::cout << tmp << '\n';
}

double as_double(long long int val) {
  double tmp = 0;

  memcpy(&tmp, &val, sizeof(long long int));

  return tmp;
}

void push_back(std::vector<long long int> *v, double val) {
  long int tmp = 0;

  memcpy(&tmp, &val, sizeof(double));

  v->push_back(tmp);
}

int main() {
  // Create a vector containing integers
  std::vector<long long int> v;

  push_back(&v, 1.1);
  push_back(&v, 2.2);
  push_back(&v, 3.3);
  push_back(&v, 4.4);
  push_back(&v, 5.5);

  std::vector<long long int> n;

  n = v;

  int position = _FAQAS_selectItem();

  for (std::vector<long long int>::iterator it = v.begin(); it != v.end();
       ++it) {

    print_double(*it);
  }

  std::cout << "Mutation\n";

  // MANUALLY ADDED PROBE
  mutate_FM_IfHK(&v);
  // MANUALLY ADDED PROBE END

  for (std::vector<long long int>::iterator it = v.begin(); it != v.end();
       ++it) {

    print_double(*it);
  }

  return 0;
}
