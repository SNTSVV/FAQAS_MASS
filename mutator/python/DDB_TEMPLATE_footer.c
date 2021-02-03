//
// Copyright (c) University of Luxembourg 2020.
// Created by Fabrizio PASTORE, fabrizio.pastore@uni.lu, SnT, 2020.
// Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
//

int _FAQAS_mutate(BUFFER_TYPE *data, FaultModel *fm) {
  if (_FAQAS_mutated == 1)
    return 0;

  if (MUTATION == -1)
    return 0;

  if (MUTATION == -2) {
    FILE *f = fopen("/home/csp/logging.txt", "ab+");
    fprintf(f, "fm.ID: %d\n", fm->ID);
    fclose(f);

    return 0;
  }

  int pos = _FAQAS_selectItem();
  int op = _FAQAS_selectOperator();
  int opt = _FAQAS_selectOperation();

  // FIXME: handle items spanning over multiple array positions
  int valueInt;
  int valueBin;
  double valueDouble;

  //
  // Load the data
  //
  if (fm->items[pos].type == BIN) {
    valueBin = (int)data[pos];
  }
  if (fm->items[pos].type == INT) {
    valueInt = (int)data[pos];
  }
  if (fm->items[pos].type == DOUBLE) {
    valueDouble = (double)data[pos];
  }

  MutationOperator *OP = &(fm->items[pos].operators[op]);

  if (OP->type == BF) {
    // FIXME: handle min-max
    int mask = 1; // 00000011

    valueBin = valueBin ^ mask; // 00000100

    _FAQAS_mutated = 1;
  }

  if (OP->type == VOR) {
    // FIXME: handle different types
    //

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
  }

  if (OP->type == VAT) {
    // FIXME: handle different types
    //

    if (fm->items[pos].type == INT) {

      valueInt = OP->threshold + OP->delta;
    }

    _FAQAS_mutated = 1;
  }

  if (OP->type == SS) {
    // FIXME: handle different types
    //

    if (fm->items[pos].type == INT) {

      int limit = OP->threshold;
      int shift = OP->delta;

      if (valueInt >= limit) {
        valueInt = valueInt - shift;
      } else {
        valueInt = valueInt + shift;
      }
    }

    _FAQAS_mutated = 1;
  }

  if (OP->type == VBT) {
    // FIXME: handle different types
    //

    if (fm->items[pos].type == INT) {

      valueInt = OP->threshold - OP->delta;
    }

    _FAQAS_mutated = 1;
  }

  if (OP->type == IV) {
    // FIXME: handle different types
    //
    if (fm->items[pos].type == INT) {

      valueInt = OP->value;
    }

    _FAQAS_mutated = 1;
  }

  if (OP->type == INV) {
    // FIXME: handle different types
    //
    srand(time(0));

    if (fm->items[pos].type == INT) {

      int upper = OP->max;
      int lower = OP->min;

      int num = (rand() % (upper - lower + 1)) + lower;
      valueInt = num;
    }

    _FAQAS_mutated = 1;
  }

  if (_FAQAS_mutated != 1) {
    return 0;
  }

  //
  // Store the data
  //
  // FIXME: handle span
  if (fm->items[pos].type == INT) {
    data[pos] = valueInt;
  }
  if (fm->items[pos].type == DOUBLE) {
    data[pos] = valueDouble;
  }
  if (fm->items[pos].type == BIN) {
    data[pos] = valueBin;
  }

  return _FAQAS_mutated;
}
