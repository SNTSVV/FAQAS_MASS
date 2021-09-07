#include "FAQAS_dataDrivenMutator.h"
#include <iostream>
#include <stdlib.h>
#include <vector>

int mutate(std::vector<int> *v, FaultModel *fm) {
  return _FAQAS_mutate(v->data(), fm);
}

int main()

{

  // Create a vector containing integers

  std::vector<int> v;

  std::vector<int> g;

  v.push_back(1);

  v.push_back(2);

  v.push_back(3);

  v.push_back(4);

  v.push_back(5);

  g.push_back(100);

  g.push_back(200);

  g.push_back(300);

  g.push_back(400);

  g.push_back(500);

  // MANUALLY ADDED PROBE 1

  FaultModel *fm1 = _FAQAS_IfHK_FM();

  mutate(&v, fm1);

  _FAQAS_delete_FM(fm1);

  // MANUALLY ADDED PROBE 1 END

  // MANUALLY ADDED PROBE 2

  FaultModel *fm69 = _FAQAS_IfTT_FM();

  mutate(&g, fm69);

  _FAQAS_delete_FM(fm69);

  // MANUALLY ADDED PROBE 2 END

  for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {

    std::cout << *it << '\n';
  }

  for (std::vector<int>::iterator it = g.begin(); it != g.end(); ++it) {

    std::cout << *it << '\n';
  }

  return 0;
}
