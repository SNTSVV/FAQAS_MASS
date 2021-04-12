#include "FAQAS_dataDrivenMutator.h"
#include <assert.h> /* assert */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// this test will be used for INV operators

int mutate(std::vector<long int> *v, FaultModel *fm) {
  return _FAQAS_mutate(v->data(), fm);
}

void print_double(long int val) {
  double tmp = 0;

  memcpy(&tmp, &val, sizeof(long int));

  std::cout << tmp << '\n';
}

double as_double(long int val) {

  double tmp = 0;

  memcpy(&tmp, &val, sizeof(long int));

  return tmp;
}

void push_back(std::vector<long int> *v, double val) {
  long int tmp = 0;

  memcpy(&tmp, &val, sizeof(double));

  v->push_back(tmp);
}

int main() {

  std::vector<long int> v;

  push_back(&v, 1.1);
  push_back(&v, 2.2);
  push_back(&v, 3.3);
  push_back(&v, 4.4);
  push_back(&v, 5.5);

  std::vector<long int> n;

  n = v;

  int position = _FAQAS_selectItem();

  for (std::vector<long int>::iterator it = v.begin(); it != v.end(); ++it) {

    print_double(*it);
  }

  std::cout << "Mutation\n";

  // MANUALLY ADDED PROBE
  FaultModel *fm = _FAQAS_IfHK_FM();
  mutate(&v, fm);
  _FAQAS_delete_FM(fm);
  // MANUALLY ADDED PROBE END

  for (std::vector<long int>::iterator it = v.begin(); it != v.end(); ++it) {

    print_double(*it);
  }

  if (position != -999) {
    for (int j = 0; j < 5; j++) {
      double a = as_double(v.at(j));
      double b = as_double(n.at(j));
      assert(faqas_double_equal(a, b));
    }
  }

  return 0;
}
