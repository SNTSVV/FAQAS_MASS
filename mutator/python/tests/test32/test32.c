//
#include "SUTlib.h"

int main() {

  //*****************************************************************************
  // these are the initial conditions for the state variables

  vectorA.clear();
  vectorA.push_back(103);
  vectorA.push_back(-223);
  vectorA.push_back(532);
  vectorA.push_back(87);
  vectorA.push_back(-278);

  vectorB.clear();
  vectorB.push_back(103.2);
  vectorB.push_back(-134.78);
  vectorB.push_back(532.897);
  vectorB.push_back(-23.7);
  vectorB.push_back(100);

  vectorC.push_back(188.99);
  vectorC.push_back(178.678);
  vectorC.push_back(-567);
  vectorC.push_back(444.56);
  vectorC.push_back(-42);

  //*****************************************************************************
  // these are the min and max values that delimit  the range in which every
  // variable should be after the execution of this script

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

  //**************************************************************************
  //**************************************************************************
  //**************************************************************************
  //**************************************************************************

  // this value changes the behaviour of the script and consequently what
  // functions are called upon from SUT.lib

  int controlType = 0;

  // 0=feedback loop: both sensors and actuators are active.
  // 1=feedforward: actuators are active but not sensors.
  // 2=monitor: only sensors are active.

  //**************************************************************************
  //**************************************************************************
  //**************************************************************************
  //**************************************************************************

  // this variable specify the number of control cycles that the script will
  // perform before stopping

  int cycles = 10;

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

  //**********feedback****************************************************

  if (controlType == 0) {

    while (cycles > 0) {

      bufferMain.clear();

      sensorA();

      int variablePosition;

      variablePosition = 0;
      while (variablePosition < 5) {

        int target =
            (int)((minA[variablePosition] + maxA[variablePosition]) / 2);

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

        float target =
            (float)((minB[variablePosition] + maxB[variablePosition]) / 2);

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

        double target =
            (double)((minC[variablePosition] + maxC[variablePosition]) / 2);

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

    std::vector<int> initialConditionsA = vectorA;
    std::vector<float> initialConditionsB = vectorB;
    std::vector<double> initialConditionsC = vectorC;

    while (cycles > 0) {

      bufferMain.clear();

      int variablePosition;

      variablePosition = 0;
      while (variablePosition < 5) {

        int target =
            (int)((minA[variablePosition] + maxA[variablePosition]) / 2);

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

        float target =
            (float)((minB[variablePosition] + maxB[variablePosition]) / 2);

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

        double target =
            (double)((minC[variablePosition] + maxC[variablePosition]) / 2);

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
