//max MUTATIONOPT=30//
// Copyright (c) University of Luxembourg 2020.
// Created by Fabrizio PASTORE, fabrizio.pastore@uni.lu, SnT, 2020.
// Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//time.h is  included for seeding the INV random number generator in the footer
#include <time.h>

//math.h is included for the INV (normal distribution) and for the BF operator
#include <math.h>

#define MAX_OPS 10
#define ITEMS 10
//#define MUTATIONOPT 2

int MUTATION=MUTATIONOPT;

// #typedef T_LONG long long

double faqas_abs(double a){
	if ( a < 0 )
		return - a;
	return a;
}

int faqas_double_equal(double a, double b){
	return faqas_abs(a-b) < 1E-3;
}

enum DataType {
    INT,
    FLOAT,
    DOUBLE,
    BIN,
    LONG
};

typedef enum DataType DataType;

enum MutationType{
    BF,
    IV,
    VOR,
    VAT,
    VBT,
    INV,
    SS,
		ASA,
		HV
};

typedef enum MutationType MutationType;

int _FAQAS_mutated = 0;

struct MutationOperator {
    MutationType type;

    // int min;
    // int max;
    // int threshold;
    // int delta;
    // int state;
    // int value;

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
    struct DataItem *items;
};

typedef struct FaultModel FaultModel;

struct FaultModel* _FAQAS_create_FM(int items){
    struct FaultModel *dm = (struct FaultModel *) malloc ( sizeof( *dm ) );
    dm->itemsN=items;
    dm->items = (struct DataItem *) malloc( sizeof ( struct DataItem ) * items );
    return dm;
}

void _FAQAS_delete_FM(FaultModel *dm) {
    if ( dm == 0 )
        return;
    free( dm->items );
    free( dm );
}

//memory for HV
int storedValueInt;
unsigned long long storedValueBin;
double storedValueDouble;
float storedValueFloat;

int repeatCounter;
int sample=1;


//_FAQAS_slice_it_up divides long integers in "slices" of binary to be stored in the elements of the buffer when span!=1
unsigned long long _FAQAS_slice_it_up(unsigned long long numberToSlice, int sliceStart,
                             int sliceEnd) {

  int i = sliceStart;
  unsigned long long slice = 0;
  while(i<=sliceEnd) {
    unsigned long long mask = pow(2, i);
    unsigned long long relevant = numberToSlice & mask;

    if (relevant == mask) {
      unsigned long long knife = pow(2, i - sliceStart);
      slice = slice | knife;
    }
    i=i+1;
  }
  return (slice);
}

//this function is useful for checking the binary data during debugging

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
#define SIZE_IfStatus 6
#define SIZE_IfHK 37
#define SIZE_GYTM 1
#define SIZE_MMTX 2
#define SIZE_SunSensorTM 1
#define SIZE_SSTP 1
#define SIZE_ReactionWheelTX 2
#define SIZE_SpaceCraftHK 3
#define SIZE_MagnetorquerSetPWMRSP 2


struct FaultModel* _FAQAS_IfStatus_FM(){
FaultModel *fm = _FAQAS_create_FM(SIZE_IfStatus);
fm->ID = 0;

fm->items[0].operators[0].type=BF;
fm->items[0].operators[0].min=3;
fm->items[0].operators[0].max=3;
fm->items[0].operators[0].state=-1;
fm->items[0].operators[0].value=1;

fm->items[0].operators[1].type=BF;
fm->items[0].operators[1].min=4;
fm->items[0].operators[1].max=4;
fm->items[0].operators[1].state=-1;
fm->items[0].operators[1].value=1;

fm->items[0].operators[2].type=BF;
fm->items[0].operators[2].min=5;
fm->items[0].operators[2].max=7;
fm->items[0].operators[2].state=-1;
fm->items[0].operators[2].value=1;
fm->items[0].operatorsN=3;
fm->items[0].span=1;
fm->items[0].type=BIN;

fm->items[1].operators[0].type=BF;
fm->items[1].operators[0].min=0;
fm->items[1].operators[0].max=4;
fm->items[1].operators[0].state=-1;
fm->items[1].operators[0].value=1;
fm->items[1].operatorsN=1;
fm->items[1].span=1;
fm->items[1].type=BIN;

fm->items[4].operators[0].type=BF;
fm->items[4].operators[0].min=0;
fm->items[4].operators[0].max=2;
fm->items[4].operators[0].state=-1;
fm->items[4].operators[0].value=1;

fm->items[4].operators[1].type=BF;
fm->items[4].operators[1].min=2;
fm->items[4].operators[1].max=4;
fm->items[4].operators[1].state=-1;
fm->items[4].operators[1].value=1;

fm->items[4].operators[2].type=BF;
fm->items[4].operators[2].min=5;
fm->items[4].operators[2].max=7;
fm->items[4].operators[2].state=-1;
fm->items[4].operators[2].value=1;
fm->items[4].operatorsN=3;
fm->items[4].span=1;
fm->items[4].type=BIN;

fm->items[5].operators[0].type=BF;
fm->items[5].operators[0].min=0;
fm->items[5].operators[0].max=1;
fm->items[5].operators[0].state=-1;
fm->items[5].operators[0].value=1;

fm->items[5].operators[1].type=BF;
fm->items[5].operators[1].min=2;
fm->items[5].operators[1].max=7;
fm->items[5].operators[1].state=-1;
fm->items[5].operators[1].value=1;
fm->items[5].operatorsN=2;
fm->items[5].span=1;
fm->items[5].type=BIN;
return fm;
}
struct FaultModel* _FAQAS_IfHK_FM(){
FaultModel *fm = _FAQAS_create_FM(SIZE_IfHK);
fm->ID = 1;

fm->items[12].operators[1].type=VAT;
fm->items[12].operators[1].threshold=3.6;
fm->items[12].operators[1].delta=0.1;
fm->items[12].operatorsN=2;
fm->items[12].span=2;
fm->items[12].type=DOUBLE;

fm->items[14].operators[0].type=SS;
fm->items[14].operators[0].threshold=0;
fm->items[14].operators[0].delta=0;
fm->items[14].operatorsN=1;
fm->items[14].span=2;
fm->items[14].type=DOUBLE;

fm->items[25].operators[0].type=SS;
fm->items[25].operators[0].threshold=0;
fm->items[25].operators[0].delta=0;
fm->items[25].operatorsN=1;
fm->items[25].span=2;
fm->items[25].type=INT;

fm->items[26].operators[0].type=SS;
fm->items[26].operators[0].threshold=0;
fm->items[26].operators[0].delta=0;
fm->items[26].operatorsN=1;
fm->items[26].span=2;
fm->items[26].type=INT;

fm->items[28].operators[0].type=SS;
fm->items[28].operators[0].threshold=0;
fm->items[28].operators[0].delta=0;
fm->items[28].operatorsN=1;
fm->items[28].span=2;
fm->items[28].type=DOUBLE;

fm->items[30].operators[0].type=SS;
fm->items[30].operators[0].threshold=0;
fm->items[30].operators[0].delta=0;
fm->items[30].operatorsN=1;
fm->items[30].span=2;
fm->items[30].type=DOUBLE;

fm->items[32].operators[0].type=SS;
fm->items[32].operators[0].threshold=0;
fm->items[32].operators[0].delta=0;
fm->items[32].operatorsN=1;
fm->items[32].span=2;
fm->items[32].type=DOUBLE;

fm->items[34].operators[0].type=SS;
fm->items[34].operators[0].threshold=0;
fm->items[34].operators[0].delta=0;
fm->items[34].operatorsN=1;
fm->items[34].span=2;
fm->items[34].type=DOUBLE;

fm->items[36].operators[0].type=SS;
fm->items[36].operators[0].threshold=0;
fm->items[36].operators[0].delta=0;
fm->items[36].operatorsN=1;
fm->items[36].span=2;
fm->items[36].type=DOUBLE;
return fm;
}
struct FaultModel* _FAQAS_GYTM_FM(){
FaultModel *fm = _FAQAS_create_FM(SIZE_GYTM);
fm->ID = 2;

fm->items[0].operators[0].type=BF;
fm->items[0].operators[0].min=0;
fm->items[0].operators[0].max=0;
fm->items[0].operators[0].state=-1;
fm->items[0].operators[0].value=0;
fm->items[0].operatorsN=1;
fm->items[0].span=1;
fm->items[0].type=BIN;
return fm;
}
struct FaultModel* _FAQAS_MMTX_FM(){
FaultModel *fm = _FAQAS_create_FM(SIZE_MMTX);
fm->ID = 3;

fm->items[0].operators[0].type=BF;
fm->items[0].operators[0].min=0;
fm->items[0].operators[0].max=0;
fm->items[0].operators[0].state=-1;
fm->items[0].operators[0].value=0;
fm->items[0].operatorsN=1;
fm->items[0].span=1;
fm->items[0].type=BIN;

fm->items[1].operators[0].type=BF;
fm->items[1].operators[0].min=0;
fm->items[1].operators[0].max=0;
fm->items[1].operators[0].state=0;
fm->items[1].operators[0].value=0;
fm->items[1].operatorsN=1;
fm->items[1].span=1;
fm->items[1].type=BIN;
return fm;
}
struct FaultModel* _FAQAS_SunSensorTM_FM(){
FaultModel *fm = _FAQAS_create_FM(SIZE_SunSensorTM);
fm->ID = 4;

fm->items[0].operators[0].type=SS;
fm->items[0].operators[0].threshold=0;
fm->items[0].operators[0].delta=0;
fm->items[0].operatorsN=1;
fm->items[0].span=2;
fm->items[0].type=DOUBLE;
return fm;
}
struct FaultModel* _FAQAS_SSTP_FM(){
FaultModel *fm = _FAQAS_create_FM(SIZE_SSTP);
fm->ID = 5;

fm->items[0].operators[0].type=SS;
fm->items[0].operators[0].threshold=0;
fm->items[0].operators[0].delta=0;
fm->items[0].operatorsN=1;
fm->items[0].span=1;
fm->items[0].type=INT;
return fm;
}
struct FaultModel* _FAQAS_ReactionWheelTX_FM(){
FaultModel *fm = _FAQAS_create_FM(SIZE_ReactionWheelTX);
fm->ID = 6;

fm->items[0].operators[0].type=BF;
fm->items[0].operators[0].min=0;
fm->items[0].operators[0].max=0;
fm->items[0].operators[0].state=0;
fm->items[0].operators[0].value=0;
fm->items[0].operatorsN=1;
fm->items[0].span=1;
fm->items[0].type=BIN;

fm->items[1].operators[0].type=BF;
fm->items[1].operators[0].min=0;
fm->items[1].operators[0].max=0;
fm->items[1].operators[0].state=0;
fm->items[1].operators[0].value=0;
fm->items[1].operatorsN=1;
fm->items[1].span=1;
fm->items[1].type=BIN;
return fm;
}
struct FaultModel* _FAQAS_SpaceCraftHK_FM(){
FaultModel *fm = _FAQAS_create_FM(SIZE_SpaceCraftHK);
fm->ID = 7;

fm->items[0].operators[0].type=VAT;
fm->items[0].operators[0].threshold=3.3;
fm->items[0].operators[0].delta=0;

fm->items[0].operators[1].type=VBT;
fm->items[0].operators[1].threshold=0;
fm->items[0].operators[1].delta=0;
fm->items[0].operatorsN=2;
fm->items[0].span=2;
fm->items[0].type=DOUBLE;

fm->items[2].operators[0].type=VOR;
fm->items[2].operators[0].min=0.5;
fm->items[2].operators[0].max=2.75;
fm->items[2].operators[0].delta=0.01;
fm->items[2].operatorsN=1;
fm->items[2].span=2;
fm->items[2].type=DOUBLE;
return fm;
}
struct FaultModel* _FAQAS_MagnetorquerSetPWMRSP_FM(){
FaultModel *fm = _FAQAS_create_FM(SIZE_MagnetorquerSetPWMRSP);
fm->ID = 8;

fm->items[0].operators[0].type=BF;
fm->items[0].operators[0].min=0;
fm->items[0].operators[0].max=0;
fm->items[0].operators[0].state=0;
fm->items[0].operators[0].value=0;
fm->items[0].operatorsN=1;
fm->items[0].span=1;
fm->items[0].type=BIN;

fm->items[1].operators[0].type=SS;
fm->items[1].operators[0].threshold=0;
fm->items[1].operators[0].delta=0;
fm->items[1].operatorsN=1;
fm->items[1].span=2;
fm->items[1].type=DOUBLE;
return fm;
}


int _FAQAS_selectItem(){
if ( MUTATION == 0 )
    return 0;
if ( MUTATION == 1 )
    return 0;
if ( MUTATION == 2 )
    return 0;
if ( MUTATION == 3 )
    return 1;
if ( MUTATION == 4 )
    return 4;
if ( MUTATION == 5 )
    return 4;
if ( MUTATION == 6 )
    return 4;
if ( MUTATION == 7 )
    return 5;
if ( MUTATION == 8 )
    return 5;
if ( MUTATION == 9 )
    return 12;
if ( MUTATION == 10 )
    return 14;
if ( MUTATION == 11 )
    return 25;
if ( MUTATION == 12 )
    return 26;
if ( MUTATION == 13 )
    return 28;
if ( MUTATION == 14 )
    return 30;
if ( MUTATION == 15 )
    return 32;
if ( MUTATION == 16 )
    return 34;
if ( MUTATION == 17 )
    return 36;
if ( MUTATION == 18 )
    return 0;
if ( MUTATION == 19 )
    return 0;
if ( MUTATION == 20 )
    return 1;
if ( MUTATION == 21 )
    return 0;
if ( MUTATION == 22 )
    return 0;
if ( MUTATION == 23 )
    return 0;
if ( MUTATION == 24 )
    return 1;
if ( MUTATION == 25 )
    return 0;
if ( MUTATION == 26 )
    return 0;
if ( MUTATION == 27 )
    return 2;
if ( MUTATION == 28 )
    return 2;
if ( MUTATION == 29 )
    return 0;
if ( MUTATION == 30 )
    return 1;
return -999;
}
int _FAQAS_selectOperator(){
if ( MUTATION == 0 )
    return 0;
if ( MUTATION == 1 )
    return 1;
if ( MUTATION == 2 )
    return 2;
if ( MUTATION == 3 )
    return 0;
if ( MUTATION == 4 )
    return 0;
if ( MUTATION == 5 )
    return 1;
if ( MUTATION == 6 )
    return 2;
if ( MUTATION == 7 )
    return 0;
if ( MUTATION == 8 )
    return 1;
if ( MUTATION == 9 )
    return 1;
if ( MUTATION == 10 )
    return 0;
if ( MUTATION == 11 )
    return 0;
if ( MUTATION == 12 )
    return 0;
if ( MUTATION == 13 )
    return 0;
if ( MUTATION == 14 )
    return 0;
if ( MUTATION == 15 )
    return 0;
if ( MUTATION == 16 )
    return 0;
if ( MUTATION == 17 )
    return 0;
if ( MUTATION == 18 )
    return 0;
if ( MUTATION == 19 )
    return 0;
if ( MUTATION == 20 )
    return 0;
if ( MUTATION == 21 )
    return 0;
if ( MUTATION == 22 )
    return 0;
if ( MUTATION == 23 )
    return 0;
if ( MUTATION == 24 )
    return 0;
if ( MUTATION == 25 )
    return 0;
if ( MUTATION == 26 )
    return 1;
if ( MUTATION == 27 )
    return 0;
if ( MUTATION == 28 )
    return 0;
if ( MUTATION == 29 )
    return 0;
if ( MUTATION == 30 )
    return 0;
return -999;
}
int _FAQAS_selectOperation(){
if ( MUTATION == 0 )
    return 0;
if ( MUTATION == 1 )
    return 0;
if ( MUTATION == 2 )
    return 0;
if ( MUTATION == 3 )
    return 0;
if ( MUTATION == 4 )
    return 0;
if ( MUTATION == 5 )
    return 0;
if ( MUTATION == 6 )
    return 0;
if ( MUTATION == 7 )
    return 0;
if ( MUTATION == 8 )
    return 0;
if ( MUTATION == 9 )
    return 0;
if ( MUTATION == 10 )
    return 0;
if ( MUTATION == 11 )
    return 0;
if ( MUTATION == 12 )
    return 0;
if ( MUTATION == 13 )
    return 0;
if ( MUTATION == 14 )
    return 0;
if ( MUTATION == 15 )
    return 0;
if ( MUTATION == 16 )
    return 0;
if ( MUTATION == 17 )
    return 0;
if ( MUTATION == 18 )
    return 0;
if ( MUTATION == 19 )
    return 0;
if ( MUTATION == 20 )
    return 0;
if ( MUTATION == 21 )
    return 0;
if ( MUTATION == 22 )
    return 0;
if ( MUTATION == 23 )
    return 0;
if ( MUTATION == 24 )
    return 0;
if ( MUTATION == 25 )
    return 0;
if ( MUTATION == 26 )
    return 0;
if ( MUTATION == 27 )
    return 0;
if ( MUTATION == 28 )
    return 1;
if ( MUTATION == 29 )
    return 0;
if ( MUTATION == 30 )
    return 0;
return -999;
}


#define APPLY_ONE_MUTATION 0

int FAQAS_fmCov;
void _FAQAS_fmCoverage(int fm){
    switch (fm){
    case 0:
    FAQAS_fmCov++;
    break;
    case 1:
    FAQAS_fmCov++;
    break;
    case 2:
    FAQAS_fmCov++;
    break;
    case 3:
    FAQAS_fmCov++;
    break;
    case 4:
    FAQAS_fmCov++;
    break;
    case 5:
    FAQAS_fmCov++;
    break;
    case 6:
    FAQAS_fmCov++;
    break;
    case 7:
    FAQAS_fmCov++;
    break;
    case 8:
    FAQAS_fmCov++;
    break;
    default:
    break;
    }
}
//END _FAQAS_fmCoverage

//
// Copyright (c) University of Luxembourg 2020.
// Created by Fabrizio PASTORE, fabrizio.pastore@uni.lu, SnT, 2020.
// Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
//

int _FAQAS_mutate(unsigned char *data, FaultModel *fm) {
  if (APPLY_ONE_MUTATION && _FAQAS_mutated == 1)
    // if (_FAQAS_mutated == 1)
    return 0;

  if (MUTATION == -1)
    return 0;

  if (MUTATION == -2) {
    _FAQAS_fmCoverage(fm->ID);

 //   const char* faqas_coverage_file = getenv("FAQAS_COVERAGE_FILE");
//    FILE *f = fopen(faqas_coverage_file, "ab+");
//
    FILE *f = fopen ("/home/svf/logging.txt", "ab+");
    fprintf(f, "fm.ID: %d\n", fm->ID);
    fclose(f);

    return 0;
  }

  if (MUTATION == -3) {
    _FAQAS_fmCoverage(fm->ID);
    return 0;
  }

  int pos = _FAQAS_selectItem();
  int op = _FAQAS_selectOperator();
  int opt = _FAQAS_selectOperation();

  int valueInt = 0;
  // int valueBin = 0;
  unsigned long long valueBin = 0;
  double valueDouble = 0;
  float valueFloat = 0;

  srand(time(NULL));
  //
  // Load the data
  //

  int span = fm->items[pos].span;

  // if (span == 1) {
  //
  //   if (fm->items[pos].type == BIN) {
  //     valueBin = (int)data[pos];
  //   }
  //   if (fm->items[pos].type == INT) {
  //
  //     valueInt = (int)data[pos];
  //   }
  //   if (fm->items[pos].type == DOUBLE) {
  //     valueDouble = (double)data[pos];
  //   }
  //   if (fm->items[pos].type == FLOAT) {
  //     valueFloat = (float)data[pos];
  //   }
  // }

  // else if (span != 1) {

  int kk;
  int step;
  unsigned long long row = 0;
  unsigned long long intermediate = 0;

  for (kk = 0; kk < (span); kk = kk + 1) {

    step = 8 * sizeof(data[pos + kk]);

    intermediate = intermediate << step;

    row = 0;

    memcpy(&row, &data[pos + kk], sizeof(data[pos + kk]));

    intermediate = (intermediate | row);
  }

  if (fm->items[pos].type == BIN) {

    unsigned long long fitToSize = (unsigned long long)intermediate;

    memcpy(&valueBin, &fitToSize, sizeof(valueBin));
  }

  if (fm->items[pos].type == INT) {

    unsigned int fitToSize = (unsigned int)intermediate;

    memcpy(&valueInt, &fitToSize, sizeof(valueInt));
  }

  if (fm->items[pos].type == DOUBLE) {

    unsigned long long int fitToSize = (unsigned long long int)intermediate;

    memcpy(&valueDouble, &fitToSize, sizeof(valueDouble));
  }

  if (fm->items[pos].type == FLOAT) {

    unsigned long int fitToSize = (unsigned long int)intermediate;

    memcpy(&valueFloat, &fitToSize, sizeof(valueFloat));
  }

  // }
  //  if else closing bracket

  MutationOperator *OP = &(fm->items[pos].operators[op]);


  if (OP->type == HV) {

    if (sample==1){

      if (fm->items[pos].type == INT) {

        storedValueInt = valueInt;
      }

      if (fm->items[pos].type == DOUBLE) {

        storedValueDouble = valueDouble;
      }

      if (fm->items[pos].type == FLOAT) {

        storedValueFloat = valueFloat;
      }

      if (fm->items[pos].type == BIN) {

        storedValueBin = valueBin;
      }

      sample=0;

      repeatCounter = OP->value;

    }

    if(repeatCounter>0){

      if (fm->items[pos].type == INT) {

        valueInt = storedValueInt;
      }

      if (fm->items[pos].type == DOUBLE) {

        valueDouble = storedValueDouble;
      }

      if (fm->items[pos].type == FLOAT) {

        valueFloat = storedValueFloat;
      }

      if (fm->items[pos].type == BIN) {

        valueBin = storedValueBin;
      }

      repeatCounter=repeatCounter-1;
    }

    if(repeatCounter==0){
      sample=1;
    }

    _FAQAS_mutated = 1;
  }


  if (OP->type == BF) {

    unsigned long long mask;
    // min = position of the first flippable bit from right to left
    int Min = OP->min;
    // max = position of the last flippable bit from right to left
    int Max = OP->max;
    // numberOfBits: (maximum) number of bits to change

    int numberOfBits = OP->value;
    // state: 1 mutate only bits ==1 and viceversa
    int State = OP->state;
    // random position of the bit to be changed
    int randomPosition;

    unsigned long long flipped;

    int avoidInfinite;

    if (State == 0) {

      // printf("entra nello state 0\n" );

      int ii = 0;

      for (ii = 0; ii < numberOfBits; ii = ii + 1) {

        avoidInfinite = 0;

        flipped = valueBin;

        while (flipped == valueBin) {

          randomPosition = (rand() % (Max - Min + 1)) + Min;

          mask = pow(2, randomPosition);

          flipped = valueBin | mask;

          avoidInfinite = avoidInfinite + 1;

          if (avoidInfinite == numberOfBits * 10) {
            break;
          }
        }

        valueBin = flipped;
      }
    }

    else if (State == 1) {

      int ii = 0;

      for (ii = 0; ii < numberOfBits; ii = ii + 1) {

        avoidInfinite = 0;

        flipped = valueBin;

        while (flipped == valueBin) {

          randomPosition = (rand() % (Max - Min + 1)) + Min;

          mask = pow(2, randomPosition);

          flipped = valueBin & ~mask;

          avoidInfinite = avoidInfinite + 1;

          if (avoidInfinite == numberOfBits * 10) {
            break;
          }
        }

        valueBin = flipped;
      }
    }

    // else if (State==-1)
    else {

      // mask = 1;                   // 00000011
      // valueBin = valueBin ^ mask; // 00000100

      int ii = 0;

      for (ii = 0; ii < numberOfBits; ii = ii + 1) {

        flipped = valueBin;

        avoidInfinite = 0;

        while (flipped == valueBin) {

          randomPosition = (rand() % (Max - Min + 1)) + Min;

          mask = pow(2, randomPosition);

          flipped = valueBin & ~mask;

          if (flipped == valueBin) {

            flipped = valueBin | mask;
          }

          avoidInfinite = avoidInfinite + 1;

          if (avoidInfinite == numberOfBits * 10)
            break;
        }

        valueBin = flipped;
      }
    }

    _FAQAS_mutated = 1;
  }

  if (OP->type == VOR) {

    if (fm->items[pos].type == INT) {

      if (opt == 0) {

        valueInt = OP->min - OP->delta;
      }

      else if (opt == 1) {
        valueInt = OP->max + OP->delta;
      }

      else {
        // ERROR
      }

      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE) {

      if (opt == 0) {

        valueDouble = (double)OP->min - OP->delta;
      }

      else if (opt == 1) {
        valueDouble = (double)OP->max + OP->delta;
      }

      else {
        // ERROR
      }

      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == FLOAT) {

      if (opt == 0) {

        valueFloat = (float)OP->min - OP->delta;
      }

      else if (opt == 1) {
        valueFloat = (float)OP->max + OP->delta;
      }

      else {
        // ERROR
      }

      _FAQAS_mutated = 1;
    }
  }

  if (OP->type == VAT) {

    if (fm->items[pos].type == INT) {

      valueInt = OP->threshold + OP->delta;

      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE) {

      valueDouble = (double)OP->threshold + OP->delta;

      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == FLOAT) {

      valueFloat = (float)OP->threshold + OP->delta;

      _FAQAS_mutated = 1;
    }
  }

  if (OP->type == VBT) {

    if (fm->items[pos].type == INT) {

      valueInt = OP->threshold - OP->delta;

      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE) {

      valueDouble = (double)OP->threshold - OP->delta;

      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == FLOAT) {

      valueFloat = (float)OP->threshold - OP->delta;

      _FAQAS_mutated = 1;
    }
  }

  if (OP->type == IV) {

    if (fm->items[pos].type == INT) {

      valueInt = OP->value;
    }

    if (fm->items[pos].type == DOUBLE) {

      valueDouble = (double)OP->value;
    }

    if (fm->items[pos].type == FLOAT) {

      valueFloat = (float)OP->value;
    }

    _FAQAS_mutated = 1;
  }

  if (OP->type == SS) {

    if (fm->items[pos].type == INT) {

      int shift = OP->delta;

      valueInt = (int)valueInt + shift;
    }

    if (fm->items[pos].type == DOUBLE) {

      double shift = OP->delta;

      valueDouble = (double)valueDouble + shift;
    }

    if (fm->items[pos].type == FLOAT) {

      float shift = OP->delta;

      valueFloat = (float)valueFloat + shift;
    }

    _FAQAS_mutated = 1;
  }


  if (OP->type == INV) {

    if (fm->items[pos].type == INT) {

      int upper = OP->max;

      int lower = OP->min;

      if (upper == lower) {

        valueInt = upper;
        // FIXME: throw a warning
      }

      else if (upper < lower) {
        // FIXME: throw an error
      }

      else {

        int randomNum = valueInt;

        int avoidInfinite = 0;

        while (valueInt == randomNum) {

          randomNum = (rand() % (upper - lower + 1)) + lower;

          avoidInfinite = avoidInfinite + 1;

          if (avoidInfinite == 1000) {

            randomNum = upper;

            break;
          }
        }

        valueInt = randomNum;
      }
    }

    if (fm->items[pos].type == DOUBLE) {

      double upper = OP->max;

      double lower = OP->min;

      if (upper == lower) {

        valueDouble = upper;
        // FIXME: throw a warning
      }

      else if (upper < lower) {
        // FIXME: throw an error
      }

      else {

        double randomNum = valueDouble;

        int avoidInfinite = 0;

        while (valueDouble == randomNum) {

          randomNum = ((double)rand() * (upper - lower)) / RAND_MAX + lower;

          avoidInfinite = avoidInfinite + 1;

          if (avoidInfinite == 1000) {

            randomNum = upper;

            break;
          }
        }

        valueDouble = randomNum;
      }
    }

    if (fm->items[pos].type == FLOAT) {

      float upper = OP->max;

      float lower = OP->min;

      if (upper == lower) {

        valueFloat = upper;
        // FIXME: throw a warning
      }

      else if (upper < lower) {
        // FIXME: throw an error
      }

      else {

        float randomNum = valueFloat;

        int avoidInfinite = 0;

        while (valueFloat == randomNum) {

          randomNum = ((float)rand() * (upper - lower)) / RAND_MAX + lower;

          avoidInfinite = avoidInfinite + 1;

          if (avoidInfinite == 1000) {

            randomNum = upper;

            break;
          }
        }

        valueFloat = randomNum;
      }
    }

    _FAQAS_mutated = 1;
  }

  if (OP->type == ASA) {

    if (fm->items[pos].type == INT) {

      int Tr = OP->threshold;

      int De = OP->delta;

      int Va = OP->value;

      if (valueInt >= Tr) {

        valueInt = Tr + ((valueInt - Tr) * Va) + De;
      }

      if (valueInt < Tr) {

        valueInt = Tr - ((valueInt - Tr) * Va) + De;
      }

      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE) {

      double Tr = OP->threshold;

      double De = OP->delta;

      double Va = OP->value;

      if (valueDouble >= Tr) {

        valueDouble = Tr + ((valueDouble - Tr) * Va) + De;
      }

      if (valueDouble < Tr) {

        valueDouble = Tr - ((valueDouble - Tr) * Va) + De;
      }

      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == FLOAT) {

      float Tr = OP->threshold;

      float De = OP->delta;

      float Va = OP->value;

      if (valueFloat >= Tr) {

        valueFloat = Tr + ((valueFloat - Tr) * Va) + De;
      }

      if (valueFloat < Tr) {

        valueFloat = Tr - ((valueFloat - Tr) * Va) + De;
      }

      _FAQAS_mutated = 1;
    }
  }



  if (_FAQAS_mutated != 1) {

    return 0;
  }

  // Store the data
  //

  // if (span == 1) {
  //
  //   if (fm->items[pos].type == INT) {
  //     data[pos] = valueInt;
  //   }
  //   if (fm->items[pos].type == DOUBLE) {
  //     data[pos] = valueDouble;
  //   }
  //   if (fm->items[pos].type == BIN) {
  //     data[pos] = valueBin;
  //   }
  //   if (fm->items[pos].type == FLOAT) {
  //     data[pos] = valueFloat;
  //   }
  //
  // }

  // else if (span != 1) {

  unsigned long long fullNumber = 0;

  switch (fm->items[pos].type) {

  case BIN:

    memcpy(&fullNumber, &valueBin, sizeof(valueBin));

    break;

  case INT:

    memcpy(&fullNumber, &valueInt, sizeof(valueInt));

    break;

  case DOUBLE:

    memcpy(&fullNumber, &valueDouble, sizeof(valueDouble));

    break;

  case FLOAT:

    memcpy(&fullNumber, &valueFloat, sizeof(valueFloat));

    break;

  case LONG:

    break;
  }

  int counter = 0;

  while (counter < span) {

    step = 8 * sizeof(data[pos + counter]);

    int startSlice = (span - counter - 1) * step;

    int endSlice = (span - counter) * step - 1;

    unsigned long long slice =
        _FAQAS_slice_it_up(fullNumber, startSlice, endSlice);

    memcpy(&data[pos + counter], &slice, sizeof(data[pos + counter]));

    counter = counter + 1;
  }

  // }
  // else if closing bracket

  return _FAQAS_mutated;
}
