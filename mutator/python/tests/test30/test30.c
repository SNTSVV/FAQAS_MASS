#include "FAQAS_dataDrivenMutator.h"
#include <assert.h> /* assert */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

int mutate(std::vector<int> *v, FaultModel *fm) {
  return _FAQAS_mutate(v->data(), fm);
}

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
  int tmp = 0;

  memcpy(&tmp, &val, sizeof(float));

  v->push_back(tmp);
}

int main() {

  std::vector<int> v;

  push_back(&v, 1);
  push_back(&v, 2);
  push_back(&v, 3);
  push_back(&v, 4);
  push_back(&v, 5);

  std::vector<int> n;

  n = v;

  int position = _FAQAS_selectItem();

  // MANUALLY ADDED PROBE
  FaultModel *fm = _FAQAS_IfHK_FM();
  mutate(&v, fm);
  _FAQAS_delete_FM(fm);
  // MANUALLY ADDED PROBE END

  for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {

    print_float(*it);
  }

  return 0;
}
