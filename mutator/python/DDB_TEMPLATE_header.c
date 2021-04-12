//
// Copyright (c) University of Luxembourg 2020.
// Created by Fabrizio PASTORE, fabrizio.pastore@uni.lu, SnT, 2020.
// Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
// Modified by Enrico VIGANO', enrico.vigano@uni.lu, SnT, 2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// time.h is  included for seeding the INV random number generator in the footer
#include <time.h>

// math.h is included for the INV (normal distribution) and for the BF operator
#include <math.h>

#define MAX_OPS 200
#define ITEMS 10

int MUTATION = MUTATIONOPT;

double faqas_abs(double a) {
  if (a < 0)
    return -a;
  return a;
}

int faqas_double_equal(double a, double b) { return faqas_abs(a - b) < 1E-3; }

enum DataType { INT, FLOAT, DOUBLE, BIN, LONG };

typedef enum DataType DataType;

enum MutationType { BF, IV, VOR, VAT, VBT, INV, SS, ASA, HV };

typedef enum MutationType MutationType;

int _FAQAS_mutated = 0;

struct MutationOperator {
  MutationType type;

  double min;
  double max;
  double threshold;
  double delta;
  int state;
  double value;
};

typedef struct MutationOperator MutationOperator;

struct DataItem {
  DataType type;
  int span;
  int operatorsN;
  struct MutationOperator operators[MAX_OPS];
};

struct FaultModel {
  int itemsN;
  int ID;
  int minOperation; // min ID of the MUTATION OPERATION implemented by this FM,
                    // included
  int maxOperation; // max ID of the MUTATION OPERATION implemented by this FM,
                    // included
  struct DataItem *items;
};

typedef struct FaultModel FaultModel;

struct FaultModel *_FAQAS_create_FM(int items) {

  struct FaultModel *dm = new struct FaultModel;

  dm->itemsN = items;

  dm->items = new DataItem[items];

  return dm;
}

void _FAQAS_delete_FM(FaultModel *dm) {
  if (dm == 0)
    return;

  delete[] dm->items;
  dm->items = 0;

  delete dm;
  dm = 0;
}

// memory for HV
int storedValueInt;
unsigned long long storedValueBin;
double storedValueDouble;
float storedValueFloat;

int repeatCounter;
int sample = 1;

//_FAQAS_slice_it_up divides long integers in "slices" of binary to be stored in
//the elements of the buffer when span!=1
unsigned long long _FAQAS_slice_it_up(unsigned long long numberToSlice,
                                      int sliceStart, int sliceEnd) {

  int i = sliceStart;
  unsigned long long slice = 0;
  while (i <= sliceEnd) {
    unsigned long long mask = pow(2, i);
    unsigned long long relevant = numberToSlice & mask;

    if (relevant == mask) {
      unsigned long long knife = pow(2, i - sliceStart);
      slice = slice | knife;
    }
    i = i + 1;
  }
  return (slice);
}

// this function is useful for checking the binary data during debugging

void _FAQAS_print_binary(unsigned long long n) {

  int steps = 8 * sizeof(n) - 1;

  while (steps >= 0) {

    unsigned long long mask = pow(2, steps);

    unsigned long long relevant = n & mask;

    if (relevant == mask) {

      printf("1");

    } else {

      printf("0");
    }

    steps = steps - 1;
  }

  printf("\n");
}
