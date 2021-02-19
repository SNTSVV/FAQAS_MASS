//
#include "FAQAS_dataDrivenMutator.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

int mutate(std::vector<long long int> *v, FaultModel *fm) {
return _FAQAS_mutate(v->data(), fm);
 }

void double_push_back(std::vector<long long int> *v, double val) {

  long long int tmp = 0;

  memcpy(&tmp, &val, sizeof(double));

  v->push_back(tmp);
}

void float_push_back(std::vector<long long int> *v, float val) {
  long long int tmp = 0;

  memcpy(&tmp, &val, sizeof(float));

  v->push_back(tmp);
}

void int_push_back(std::vector<long long int> *v, int val) {
  long long int tmp = 0;

  memcpy(&tmp, &val, sizeof(int));

  v->push_back(tmp);
}

std::vector<int> vectorA;

std::vector<float> vectorB;

std::vector<double> vectorC;

std::vector<long long int> bufferA;
std::vector<long long int> bufferB;
std::vector<long long int> bufferC;
std::vector<long long int> bufferMain;

std::vector<int> minA;
std::vector<int> maxA;
std::vector<float> minB;
std::vector<float> maxB;
std::vector<double> minC;
std::vector<double> maxC;

void sensorA() {

  // legge il vettore degli int

  bufferA.clear();
  int position = 0;
  while (position < 5) {
    int_push_back(&bufferA, vectorA[position]);
    position = position + 1;
  }

  // // PROBE
  // int mutate(&bufferA, fm)
  // // END OF THE PROBE
}

void sensorB() {

  // legge il vettore dei float

  bufferB.clear();
  int position = 0;
  while (position < 5) {
    float_push_back(&bufferB, vectorB[position]);
    position = position + 1;
  }
}

void sensorC() {

  // legge il vettore dei double
  bufferC.clear();
  int position = 0;
  while (position < 5) {
    double_push_back(&bufferC, vectorC[position]);
    position = position + 1;
  }
}

void actuatorA() {
  // riceve un vettore di zeri e di valori che indica su quale variabile agire
  // con 3/5/30/50
  int add = 0;
  memcpy(&add, &bufferMain[0], sizeof(add));
  vectorA[0] = vectorA[0] + add;
  add = 0;
  memcpy(&add, &bufferMain[1], sizeof(add));
  vectorA[1] = vectorA[1] + add;
  add = 0;
  memcpy(&add, &bufferMain[2], sizeof(add));
  vectorA[2] = vectorA[2] + add;
  add = 0;
  memcpy(&add, &bufferMain[3], sizeof(add));
  vectorA[3] = vectorA[3] + add;
  add = 0;
  memcpy(&add, &bufferMain[4], sizeof(add));
  vectorA[4] = vectorA[4] + add;
  bufferMain.clear();
}

void actuatorB() {
  // riceve un vettore di zeri e di valori che indica su quale variabile agire
  // con 3/5/30/50
  int add = 0;
  memcpy(&add, &bufferMain[0], sizeof(add));
  vectorB[0] = vectorB[0] + add;
  add = 0;
  memcpy(&add, &bufferMain[1], sizeof(add));
  vectorB[1] = vectorB[1] + add;
  add = 0;
  memcpy(&add, &bufferMain[2], sizeof(add));
  vectorB[2] = vectorB[2] + add;
  add = 0;
  memcpy(&add, &bufferMain[3], sizeof(add));
  vectorB[3] = vectorB[3] + add;
  add = 0;
  memcpy(&add, &bufferMain[4], sizeof(add));
  vectorB[4] = vectorB[4] + add;
  bufferMain.clear();
}

void actuatorC() {
  // riceve un vettore di zeri e di valori che indica su quale variabile agire
  // con 3/5/30/50
  int add = 0;
  memcpy(&add, &bufferMain[0], sizeof(add));
  vectorC[0] = vectorC[0] + add;
  add = 0;
  memcpy(&add, &bufferMain[1], sizeof(add));
  vectorC[1] = vectorC[1] + add;
  add = 0;
  memcpy(&add, &bufferMain[2], sizeof(add));
  vectorC[2] = vectorC[2] + add;
  add = 0;
  memcpy(&add, &bufferMain[3], sizeof(add));
  vectorC[3] = vectorC[3] + add;
  add = 0;
  memcpy(&add, &bufferMain[4], sizeof(add));
  vectorC[4] = vectorC[4] + add;
  bufferMain.clear();
}

int getCorrection(int error) {

  int absoluteError = abs(error);
  // e in base a quanto é sceglie il comando da applicare
  int absCorrection = 0;

  if (absoluteError == 0) {
    absCorrection = 0;
  }

  else if (absoluteError > 0 && absoluteError < 5) {
    absCorrection = 1;
  }

  else if (absoluteError >= 5 && absoluteError < 10) {
    absCorrection = 5;
  }

  else if (absoluteError >= 10 && absoluteError < 50) {
    absCorrection = 10;
  }

  else {
    absCorrection = 100;
  }

  int correction;

  if (error <= 0) {

    correction = -absCorrection;
  }

  if (error > 0) {
    correction = absCorrection;
  }

  return correction;
}

int main() {

  // questo poi sarà preso da un csv

  vectorA.clear();
  vectorA.push_back(103);
  vectorA.push_back(-223);
  vectorA.push_back(532);
  vectorA.push_back(87);
  vectorA.push_back(-278);

  maxA.push_back(50);
  maxA.push_back(50);
  maxA.push_back(50);
  maxA.push_back(50);
  maxA.push_back(50);

  minA.push_back(-20);
  minA.push_back(-20);
  minA.push_back(-20);
  minA.push_back(-20);
  minA.push_back(-20);

  vectorB.clear();
  vectorB.push_back(103.2);
  vectorB.push_back(-134.78);
  vectorB.push_back(532.897);
  vectorB.push_back(-23.7);
  vectorB.push_back(100);

  maxB.push_back(50);
  maxB.push_back(50);
  maxB.push_back(50);
  maxB.push_back(50);
  maxB.push_back(50);

  minB.push_back(-20);
  minB.push_back(-20);
  minB.push_back(-20);
  minB.push_back(-20);
  minB.push_back(-20);

  vectorC.push_back(188.99);
  vectorC.push_back(178.678);
  vectorC.push_back(-567);
  vectorC.push_back(444.56);
  vectorC.push_back(-42);

  maxC.push_back(50);
  maxC.push_back(50);
  maxC.push_back(50);
  maxC.push_back(50);
  maxC.push_back(50);

  minC.push_back(-20);
  minC.push_back(-20);
  minC.push_back(-20);
  minC.push_back(-20);
  minC.push_back(-20);

  // // MANUALLY ADDED PROBE
  // FaultModel *fm = _FAQAS_IfHK_FM();
  // mutate(&v, fm);
  // // MANUALLY ADDED PROBE END

  //**************************************************************************
  //**************************************************************************
  //**************************************************************************
  //**************************************************************************

  int controlType = 2;

  // 0=feedback loop: both sensors and actuators are active.
  // 1=feedforward: actuators are active but non sensors.
  // 2=monitor: only sensors are active.

  //**************************************************************************
  //**************************************************************************
  //**************************************************************************
  //**************************************************************************

  printf("vectorA before\n");

  for (std::vector<int>::iterator it = vectorA.begin(); it != vectorA.end();
       ++it) {
    std::cout << *it << '\n';
  }

  printf("vectorB before\n");

  for (std::vector<float>::iterator it = vectorB.begin(); it != vectorB.end();
       ++it) {
    std::cout << *it << '\n';
  }

  printf("vectorC before\n");

  for (std::vector<double>::iterator it = vectorC.begin(); it != vectorC.end();
       ++it) {
    std::cout << *it << '\n';
  }

  int cycles = 10;

  //**********feedback****************************************************

  if (controlType == 0) {

    while (cycles > 0) {

      // prende il vettore dai sensori e legge le variabili int

      bufferMain.clear();

      sensorA();

      int variablePosition;

      variablePosition = 0;
      while (variablePosition < 5) {

        // prende il range, fa la media e lo considera il suo obiettivo

        int target =
            (int)((minA[variablePosition] + maxA[variablePosition]) / 2);

        // obbiettivo-variabile e ottiene l'errore

        int variable = 0;
        memcpy(&variable, &bufferA[variablePosition], sizeof(variable));

        int error = (int)(target - variable);

        int u = getCorrection(error);

        int_push_back(&bufferMain, u);
        variablePosition = variablePosition + 1;
      }

      actuatorA();

      bufferMain.clear();

      sensorB();

      variablePosition = 0;
      while (variablePosition < 5) {

        // prende il range, fa la media e lo considera il suo obiettivo

        float target =
            (float)((minB[variablePosition] + maxB[variablePosition]) / 2);

        // obbiettivo-variabile e ottiene l'errore

        float variable = 0;

        memcpy(&variable, &bufferB[variablePosition], sizeof(variable));

        int error = (int)(target - variable);

        int u = getCorrection(error);

        int_push_back(&bufferMain, u);
        variablePosition = variablePosition + 1;
      }

      actuatorB();

      bufferMain.clear();

      sensorC();

      variablePosition = 0;
      while (variablePosition < 5) {

        // prende il range, fa la media e lo considera il suo obiettivo

        double target =
            (double)((minC[variablePosition] + maxC[variablePosition]) / 2);

        // obbiettivo-variabile e ottiene l'errore

        double variable = 0;

        memcpy(&variable, &bufferC[variablePosition], sizeof(variable));

        int error = (int)(target - variable);

        int u = getCorrection(error);

        int_push_back(&bufferMain, u);
        variablePosition = variablePosition + 1;
      }

      actuatorC();

      printf("Remaining cycles: %d\n", cycles);

      printf("vectorA\n");

      for (std::vector<int>::iterator it = vectorA.begin(); it != vectorA.end();
           ++it) {
        std::cout << *it << '\n';
      }

      printf("vectorB\n");

      for (std::vector<float>::iterator it = vectorB.begin();
           it != vectorB.end(); ++it) {
        std::cout << *it << '\n';
      }

      printf("vectorC\n");

      for (std::vector<double>::iterator it = vectorC.begin();
           it != vectorC.end(); ++it) {
        std::cout << *it << '\n';
      }

      cycles = cycles - 1;
    }

    printf("vectorA after\n");

    for (std::vector<int>::iterator it = vectorA.begin(); it != vectorA.end();
         ++it) {
      std::cout << *it << '\n';
    }

    printf("vectorB after\n");

    for (std::vector<float>::iterator it = vectorB.begin(); it != vectorB.end();
         ++it) {
      std::cout << *it << '\n';
    }

    printf("vectorC after\n");

    for (std::vector<double>::iterator it = vectorC.begin();
         it != vectorC.end(); ++it) {
      std::cout << *it << '\n';
    }
  }

  //**********feedforward****************************************************

  if (controlType == 1) {

    // save the avriable vectors directly as initial initialConditions

    std::vector<int> initialConditionsA = vectorA;
    std::vector<float> initialConditionsB = vectorB;
    std::vector<double> initialConditionsC = vectorC;

    while (cycles > 0) {

      // prende il vettore dai sensori e legge le variabili int

      bufferMain.clear();

      int variablePosition;

      variablePosition = 0;
      while (variablePosition < 5) {

        // prende il range, fa la media e lo considera il suo obiettivo

        int target =
            (int)((minA[variablePosition] + maxA[variablePosition]) / 2);

        // obbiettivo-variabile e ottiene l'errore

        int error = (int)(target - initialConditionsA[variablePosition]);

        int u = getCorrection(error);

        int_push_back(&bufferMain, u);

        initialConditionsA[variablePosition] =
            initialConditionsA[variablePosition] + u;

        variablePosition = variablePosition + 1;
      }

      actuatorA();

      bufferMain.clear();

      variablePosition = 0;
      while (variablePosition < 5) {

        // prende il range, fa la media e lo considera il suo obiettivo

        float target =
            (float)((minB[variablePosition] + maxB[variablePosition]) / 2);

        // obbiettivo-variabile e ottiene l'errore

        int error = (int)(target - initialConditionsB[variablePosition]);

        int u = getCorrection(error);

        int_push_back(&bufferMain, u);

        initialConditionsB[variablePosition] =
            initialConditionsB[variablePosition] + u;

        variablePosition = variablePosition + 1;
      }

      actuatorB();

      bufferMain.clear();

      variablePosition = 0;
      while (variablePosition < 5) {

        // prende il range, fa la media e lo considera il suo obiettivo

        double target =
            (double)((minC[variablePosition] + maxC[variablePosition]) / 2);

        // obbiettivo-variabile e ottiene l'errore

        int error = (int)(target - initialConditionsC[variablePosition]);

        int u = getCorrection(error);

        int_push_back(&bufferMain, u);

        initialConditionsC[variablePosition] =
            initialConditionsC[variablePosition] + u;

        variablePosition = variablePosition + 1;
      }

      actuatorC();

      printf("Remaining cycles: %d\n", cycles);

      printf("vectorA\n");

      for (std::vector<int>::iterator it = vectorA.begin(); it != vectorA.end();
           ++it) {
        std::cout << *it << '\n';
      }

      printf("vectorB\n");

      for (std::vector<float>::iterator it = vectorB.begin();
           it != vectorB.end(); ++it) {
        std::cout << *it << '\n';
      }

      printf("vectorC\n");

      for (std::vector<double>::iterator it = vectorC.begin();
           it != vectorC.end(); ++it) {
        std::cout << *it << '\n';
      }

      cycles = cycles - 1;
    }

    printf("vectorA after\n");

    for (std::vector<int>::iterator it = vectorA.begin(); it != vectorA.end();
         ++it) {
      std::cout << *it << '\n';
    }

    printf("vectorB after\n");

    for (std::vector<float>::iterator it = vectorB.begin(); it != vectorB.end();
         ++it) {
      std::cout << *it << '\n';
    }

    printf("vectorC after\n");

    for (std::vector<double>::iterator it = vectorC.begin();
         it != vectorC.end(); ++it) {
      std::cout << *it << '\n';
    }
  }

  //**********monitor****************************************************

  if (controlType == 2) {

    while (cycles > 0) {

      // prende il vettore dai sensori e legge le variabili int

      sensorA();

      int variablePosition;

      variablePosition = 0;
      while (variablePosition < 5) {

        int variable = 0;
        memcpy(&variable, &bufferA[variablePosition], sizeof(variable));

        if (variable > maxA[variablePosition]) {
          printf("variable A%d is too low\n", variablePosition);
        }

        else if (variable < minA[variablePosition]) {
          printf("variable A%d is too high\n", variablePosition);
        }

        else {
          printf("variable A%d is in range\n", variablePosition);
        }

        variablePosition = variablePosition + 1;
      }

      sensorB();

      variablePosition = 0;
      while (variablePosition < 5) {

        float variable = 0;

        memcpy(&variable, &bufferB[variablePosition], sizeof(variable));

        if (variable > maxB[variablePosition]) {
          printf("variable B%d is too low\n", variablePosition);
        }

        else if (variable < minB[variablePosition]) {
          printf("variable B%d is too high\n", variablePosition);
        }

        else {
          printf("variable B%d is in range\n", variablePosition);
        }

        variablePosition = variablePosition + 1;
      }

      sensorC();

      variablePosition = 0;
      while (variablePosition < 5) {

        double variable = 0;

        memcpy(&variable, &bufferC[variablePosition], sizeof(variable));

        if (variable > maxC[variablePosition]) {
          printf("variable C%d is too low\n", variablePosition);
        }

        else if (variable < minC[variablePosition]) {
          printf("variable C%d is too high\n", variablePosition);
        }

        else {
          printf("variable C%d is in range\n", variablePosition);
        }

        variablePosition = variablePosition + 1;
      }

      printf("Remaining cycles: %d\n", cycles);

      printf("vectorA\n");

      for (std::vector<int>::iterator it = vectorA.begin(); it != vectorA.end();
           ++it) {
        std::cout << *it << '\n';
      }

      printf("vectorB\n");

      for (std::vector<float>::iterator it = vectorB.begin();
           it != vectorB.end(); ++it) {
        std::cout << *it << '\n';
      }

      printf("vectorC\n");

      for (std::vector<double>::iterator it = vectorC.begin();
           it != vectorC.end(); ++it) {
        std::cout << *it << '\n';
      }

      variablePosition = 0;

      while (variablePosition < 5) {

        vectorA[variablePosition] = vectorA[variablePosition] + 30;
        vectorB[variablePosition] = vectorB[variablePosition] + 30;
        vectorC[variablePosition] = vectorB[variablePosition] + 30;

        variablePosition = variablePosition + 1;
      }

      cycles = cycles - 1;
    }

    printf("vectorA after\n");

    for (std::vector<int>::iterator it = vectorA.begin(); it != vectorA.end();
         ++it) {
      std::cout << *it << '\n';
    }

    printf("vectorB after\n");

    for (std::vector<float>::iterator it = vectorB.begin(); it != vectorB.end();
         ++it) {
      std::cout << *it << '\n';
    }

    printf("vectorC after\n");

    for (std::vector<double>::iterator it = vectorC.begin();
         it != vectorC.end(); ++it) {
      std::cout << *it << '\n';
    }
  }

  return 0;
}
