#include "FAQAS_dataDrivenMutator.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

int mutate(std::vector<long int> *v, FaultModel *fm) {
  return _FAQAS_mutate(v->data(), fm);
}

//*****************************************************************************
// These 3 functions are used to write various datatypes into the buffer

void double_push_back(std::vector<long int> *v, double val) {

  long int tmp = 0;

  memcpy(&tmp, &val, sizeof(double));

  v->push_back(tmp);
}

void float_push_back(std::vector<long int> *v, float val) {
  long int tmp = 0;

  memcpy(&tmp, &val, sizeof(float));

  v->push_back(tmp);
}

void int_push_back(std::vector<long int> *v, int val) {
  long int tmp = 0;

  memcpy(&tmp, &val, sizeof(int));

  v->push_back(tmp);
}

//*****************************************************************************
// this vector represents the state variables of a system

std::vector<int> vectorA;

std::vector<float> vectorB;

std::vector<double> vectorC;

//*****************************************************************************
// this vectors represent the buffer

std::vector<long int> bufferA;
std::vector<long int> bufferB;
std::vector<long int> bufferC;
std::vector<long int> bufferMain;

//*****************************************************************************
// this vectors will contain the parameters in which the variables must remain

std::vector<int> minA;
std::vector<int> maxA;
std::vector<float> minB;
std::vector<float> maxB;
std::vector<double> minC;
std::vector<double> maxC;

//*****************************************************************************
// these functions simulate sensors acquiring the values of the state variables
// and writing them on the buffer to be read by a control system

void sensorA() {


  bufferA.clear();
  int position = 0;
  while (position < 5) {
    int_push_back(&bufferA, vectorA[position]);
    position = position + 1;
  }

  // PROBE
  FaultModel *fm = _FAQAS_SensorA_FM();
  mutate(&bufferA, fm);
  _FAQAS_delete_FM(fm);
  // END OF THE PROBE
}

void sensorB() {


  bufferB.clear();
  int position = 0;
  while (position < 5) {
    float_push_back(&bufferB, vectorB[position]);
    position = position + 1;
  }
  // PROBE
  FaultModel *fm = _FAQAS_SensorB_FM();
  mutate(&bufferA, fm);
  _FAQAS_delete_FM(fm);
  // END OF THE PROBE
}

void sensorC() {


  bufferC.clear();
  int position = 0;
  while (position < 5) {
    double_push_back(&bufferC, vectorC[position]);
    position = position + 1;
  }

  // PROBE
  FaultModel *fm = _FAQAS_SensorC_FM();
  mutate(&bufferA, fm);
  _FAQAS_delete_FM(fm);
  // END OF THE PROBE
}

//*****************************************************************************
// these functions emulate actuators acting on the state variables using the
// vector BufferMain as reference

void actuatorA() {

  int add = 0;

  // PROBE
  FaultModel *fm = _FAQAS_ActuatorA_FM();
  mutate(&bufferMain, fm);
  _FAQAS_delete_FM(fm);
  // END OF THE PROBE

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

  // PROBE
  FaultModel *fm = _FAQAS_ActuatorB_FM();
  mutate(&bufferMain, fm);
  _FAQAS_delete_FM(fm);
  // END OF THE PROBE

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

  // PROBE
  FaultModel *fm = _FAQAS_ActuatorC_FM();
  mutate(&bufferMain, fm);
  _FAQAS_delete_FM(fm);
  // END OF THE PROBE

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

//*****************************************************************************
// this functions represent a proportional controller taking as input the error
// and giving different outputs depending on the magnitude of the error

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
