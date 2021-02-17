//
// Copyright (c) University of Luxembourg 2020.
// Created by Fabrizio PASTORE, fabrizio.pastore@uni.lu, SnT, 2020.
// Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
//

int _FAQAS_mutate(BUFFER_TYPE *data, FaultModel *fm) {
  if (APPLY_ONE_MUTATION && _FAQAS_mutated == 1)
    // if (_FAQAS_mutated == 1)
    return 0;

  if (MUTATION == -1)
    return 0;

  if (MUTATION == -2) {
    _FAQAS_fmCoverage(fm->ID);
    //FILE *f = fopen("/home/csp/logging.txt", "ab+");
    //fprintf(f, "fm.ID: %d\n", fm->ID);
    //fclose(f);

    return 0;
  }

  int pos = _FAQAS_selectItem();
  int op = _FAQAS_selectOperator();
  int opt = _FAQAS_selectOperation();

  // FIXME: handle items spanning over multiple array positions

  int valueInt = 0;
  int valueBin = 0;
  double valueDouble = 0;
  float valueFloat = 0;

  srand(time(NULL));
  //
  // Load the data
  //

  int span = fm->items[pos].span;

  if (span == 1) {

    if (fm->items[pos].type == BIN) {
      valueBin = (int)data[pos];
    }
    if (fm->items[pos].type == INT) {

      valueInt = (int)data[pos];
    }
    if (fm->items[pos].type == DOUBLE) {
      valueDouble = (double)data[pos];
    }
    if (fm->items[pos].type == FLOAT) {
      valueFloat = (float)data[pos];
    }
  }

  else if (span != 1) {

    unsigned long long kk;
    unsigned long long step;
    unsigned long long row;
    unsigned long long intermediate = 0;

    for (kk = 0; kk < (span); kk = kk + 1) {

      step = 8 * sizeof(data[pos + kk]);

      row = ((unsigned long long)data[pos + kk] << (step * (span - 1 - kk)));

      intermediate = (intermediate | row);
    }


    if (fm->items[pos].type == BIN) {
      unsigned int fitToSize = (unsigned int)intermediate;
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

  }

  MutationOperator *OP = &(fm->items[pos].operators[op]);

  if (OP->type == BF) {

    int mask;
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
    int flipped;
    int avoidInfinite;

    if (State == 0) {

      int ii = 0;

      for (ii = 0; ii < numberOfBits; ii = ii + 1) {
        avoidInfinite = 0;
        flipped = valueBin;

        while (flipped == valueBin) {

          randomPosition = (rand() % (Max - Min + 1)) + Min;

          mask = (int)pow(2, randomPosition);

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

          mask = (int)pow(2, randomPosition);

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

          mask = (int)pow(2, randomPosition);

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

    // printf("entra\n" );
    if (fm->items[pos].type == INT) {
      // printf("intero\n" );
      int Tr = OP->threshold;
      int De = OP->delta;
      int Va = OP->value;

      if (valueInt >= Tr) {
        // printf("sopra t\n" );
        valueInt = Tr + ((valueInt - Tr) * Va) + De;
        // printf("nuovo value %d\n", valueInt);
      }

      if (valueInt < Tr) {
        // printf("sotto t\n" );
        valueInt = Tr - ((valueInt - Tr) * Va) + De;
        // printf("nuovo value %d\n", valueInt);
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

  if (span == 1) {

    if (fm->items[pos].type == INT) {
      data[pos] = valueInt;
    }
    if (fm->items[pos].type == DOUBLE) {
      data[pos] = valueDouble;
    }
    if (fm->items[pos].type == BIN) {
      data[pos] = valueBin;
    }
    if (fm->items[pos].type == FLOAT) {
      data[pos] = valueFloat;
    }

  }

  else if (span != 1) {

    unsigned long long fullNumber;

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

    int step = sizeof(data[pos]) * 8;
    int perry = 0;

    while (perry < span) {
      data[pos + perry] = sliceItUp(fullNumber, (span - 1 - perry) * step,
                                    (span - perry) * step);
      perry = perry + 1;
    }
  }

  return _FAQAS_mutated;
}
