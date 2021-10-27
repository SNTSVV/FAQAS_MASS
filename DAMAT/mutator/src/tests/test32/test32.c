#include <iostream>
#include <vector>
#include <stdlib.h>
#include "FAQAS_dataDrivenMutator.h"

int main(){

  // SensorA,1,1,INT,SS,NA,NA,NA,1000,NA,NA
  // SensorB,1,1,FLOAT,SS,NA,NA,NA,1000,NA,NA
  // SensorC,1,1,DOUBLE,SS,NA,NA,NA,1000,NA,NA
  // ActuatorA,1,1,INT,SS,NA,NA,NA,1000,NA,NA
  // ActuatorB,1,1,FLOAT,SS,NA,NA,NA,1000,NA,NA
  // ActuatorC,1,1,DOUBLE,SS,NA,NA,NA,1000,NA,NA
    // Create a vector containing integers
    std::vector<int> v;

    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);

    // mutate_FM_IfHK

  for (int i = 0; i < 10; i++){

    //MANUALLY ADDED PROBE
    mutate_FM_SensorA(&v);
    //MANUALLY ADDED PROBE END

  }

  for (int i = 0; i < 11; i++){

    //MANUALLY ADDED PROBE
    mutate_FM_SensorB(&v);

    //MANUALLY ADDED PROBE END

  }

  for (int i = 0; i < 12; i++){

    //MANUALLY ADDED PROBE
    mutate_FM_SensorC(&v);
    //MANUALLY ADDED PROBE END

  }

  for (int i = 0; i < 13; i++){

    //MANUALLY ADDED PROBE
    mutate_FM_ActuatorA(&v);
    //MANUALLY ADDED PROBE END

  }

  for (int i = 0; i < 14; i++){

    //MANUALLY ADDED PROBE
    mutate_FM_ActuatorB(&v);
    //MANUALLY ADDED PROBE END

  }

  for (int i = 0; i < 15; i++){

    //MANUALLY ADDED PROBE
    mutate_FM_ActuatorC(&v);
    //MANUALLY ADDED PROBE END

  }


    for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
      std::cout << *it << '\n';
    }

    return 0;
}
