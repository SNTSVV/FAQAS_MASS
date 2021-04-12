#include "FAQAS_dataDrivenMutator.h"
#include <assert.h> /* assert */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

int mutate(std::vector<float> *v, FaultModel *fm) {
  return _FAQAS_mutate(v->data(), fm);
}

void print_int(float val) {
  int tmp = 0;

  memcpy(&tmp, &val, sizeof(float));

  std::cout << tmp << '\n';
}

int as_int(float val) {

  int tmp = 0;

  memcpy(&tmp, &val, sizeof(float));

  return tmp;
}

void push_back(std::vector<float> *v, int val) {
  float tmp = 0;

  memcpy(&tmp, &val, sizeof(int));

  v->push_back(tmp);
}

int main() {

  std::vector<float> v;

  push_back(&v, 1.1);
  push_back(&v, 2.2);
  push_back(&v, 3.3);
  push_back(&v, 4.4);
  push_back(&v, 5.5);

  std::vector<float> n;

  n = v;

  int position = _FAQAS_selectItem();

  std::cout << "Mutation\n";

  // MANUALLY ADDED PROBE
  FaultModel *fm = _FAQAS_IfHK_FM();
  mutate(&v, fm);
  _FAQAS_delete_FM(fm);
  // MANUALLY ADDED PROBE END

  for (std::vector<float>::iterator it = v.begin(); it != v.end(); ++it) {

    print_int(*it);
  }

  if (position != -999) {
    for (int j = 0; j < 5; j++) {
      int a = as_int(v.at(j));
      int b = as_int(n.at(j));
      assert(faqas_int_equal(a, b));
    }
  }

  return 0;
}
