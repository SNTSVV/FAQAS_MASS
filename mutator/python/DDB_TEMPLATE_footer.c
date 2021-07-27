//
// Copyright (c) University of Luxembourg 2020.
// Created by Fabrizio PASTORE, fabrizio.pastore@uni.lu, SnT, 2020.
// Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
// Modified by Enrico VIGANO', enrico.vigano@uni.lu, SnT, 2021.
//

int _FAQAS_mutate(BUFFER_TYPE *data, FaultModel *fm) {
  if (APPLY_ONE_MUTATION && _FAQAS_mutated == 1)
    return 0;

  if (MUTATION == -1)
    return 0;

  if (MUTATION == -2) {
   _FAQAS_fmCoverage(fm->ID);
    fprintf(coverage_file_pointer, "fm.ID: %d\n", fm->ID);
    return 0;
  }

  if (MUTATION == -3) {
    _FAQAS_fmCoverage(fm->ID);
    return 0;
  }

  if (MUTATION < fm->minOperation || MUTATION >= fm->maxOperation) {
    _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
    return 0;
  }

#ifdef _FAQAS_SINGLE_MUTATION

  if (global_mutation_counter > 0){
    _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
    global_mutation_counter = global_mutation_counter + 1;
    return 0;
  }

#endif


#ifdef _FAQAS_MUTATION_PROBABILITY

  float random_check = ((float)rand() * (100)) / RAND_MAX;

  if (PROBABILITY < 0 || random_check > PROBABILITY){
    _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
    global_mutation_counter = global_mutation_counter + 1;
    return 0;
  }

#endif


  int pos = _FAQAS_selectItem();
  int data_pos = _FAQAS_INITIAL_PADDING + pos;
  int op = _FAQAS_selectOperator();
  int opt = _FAQAS_selectOperation();

  int valueInt = 0;
  long int valueLong = 0;
  unsigned long long valueBin = 0;
  double valueDouble = 0;
  float valueFloat = 0;

  srand(time(NULL));
  //
  // Load the data
  //

  int span = fm->items[pos].span;

  int kk;
  int stepRead;
  unsigned long long row = 0;
  unsigned long long intermediate = 0;

  for (kk = 0; kk < (span); kk = kk + 1) {
    stepRead = 8 * sizeof(data[data_pos + kk]);
    intermediate = intermediate << stepRead;
    row = 0;
    memcpy(&row, &data[data_pos + kk], sizeof(data[data_pos + kk]));
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

  if (fm->items[pos].type == LONG) {
    unsigned long int fitToSize = (unsigned long int)intermediate;
    memcpy(&valueLong, &fitToSize, sizeof(valueLong));
  }

  //
  // Mutate the data
  //

  MutationOperator *OP = &(fm->items[pos].operators[op]);

  if (OP->type == HV) {

    if (sample == 1) {
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

      if (fm->items[pos].type == LONG) {
        storedValueBin = valueBin;
      }

      sample = 0;
      repeatCounter = OP->value;
    }

    if (repeatCounter > 0) {

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

      if (fm->items[pos].type == LONG) {
        valueLong = storedValueLong;
      }

      repeatCounter = repeatCounter - 1;
    }

    if (repeatCounter == 0) {
      sample = 1;
    }

    _FAQAS_mutated = 1;
    _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
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
    int success = 1;

    if (State == 0) {
      int ii = 0;
      for (ii = 0; ii < numberOfBits; ii = ii + 1){
        avoidInfinite = 0;
        flipped = valueBin;

        while (flipped == valueBin) {
          randomPosition = (rand() % (Max - Min + 1)) + Min;
          mask = FAQAS_pow_substitute(2, randomPosition);
          flipped = valueBin | mask;
          avoidInfinite = avoidInfinite + 1;
          if (avoidInfinite == numberOfBits * 10){
            success = 0;
            break;
          }
        }
        valueBin = flipped;
      }
    }
    else if (State == 1) {
      int ii = 0;

      for (ii = 0; ii < numberOfBits; ii = ii + 1){
        avoidInfinite = 0;
        flipped = valueBin;
        while (flipped == valueBin){
          randomPosition = (rand() % (Max - Min + 1)) + Min;
          mask = FAQAS_pow_substitute(2, randomPosition);
          flipped = valueBin & ~mask;
          avoidInfinite = avoidInfinite + 1;
          if (avoidInfinite == numberOfBits * 10) {
            success = 0;
            break;
          }
        }
        valueBin = flipped;
      }
    }
    else {
      int ii = 0;

      for (ii = 0; ii < numberOfBits; ii = ii + 1) {
        flipped = valueBin;
        avoidInfinite = 0;

        while (flipped == valueBin){
          randomPosition = (rand() % (Max - Min + 1)) + Min;
          mask = FAQAS_pow_substitute(2, randomPosition);
          flipped = valueBin & ~mask;
          if (flipped == valueBin){
            flipped = valueBin | mask;
          }
          avoidInfinite = avoidInfinite + 1;
          if (avoidInfinite == numberOfBits * 10){
            success = 0;
            break;
          }
        }
        valueBin = flipped;
      }
    }
    _FAQAS_operator_coverage(MUTATION, global_mutation_counter, success);
    _FAQAS_mutated = 1;
  }

  if (OP->type == VOR) {

    if (fm->items[pos].type == INT) {
      if (valueInt >= OP->min && valueInt <= OP->max ) {
        if (opt == 0) {
          valueInt = OP->min - OP->delta;
          _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
        }
        else if (opt == 1) {
          valueInt = OP->max + OP->delta;
          _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
        }
        else {
          // FIXME: throw an error
        }
      }
      else {
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == LONG) {
      if (valueLong >= OP->min && valueLong <= OP->max ) {
        if (opt == 0) {
          valueLong = OP->min - OP->delta;
          _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
        }
        else if (opt == 1) {
          valueLong = OP->max + OP->delta;
          _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
        }
        else {
          // FIXME: throw an error
        }
      }
      else {
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE) {
      if (valueDouble >= OP->min && valueDouble <= OP->max ) {
        if (opt == 0) {
          valueDouble = OP->min - OP->delta;
          _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
        }
        else if (opt == 1) {
          valueDouble = OP->max + OP->delta;
          _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
        }
        else {
          // FIXME: throw an error
        }
      }
      else {
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == FLOAT) {
      if (valueFloat >= OP->min && valueFloat <= OP->max ) {
        if (opt == 0) {
          valueFloat = OP->min - OP->delta;
          _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
        }
        else if (opt == 1) {
          valueFloat = OP->max + OP->delta;
          _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
        }
        else {
          // FIXME: throw an error
        }
      }
      else {
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }
  }

  if (OP->type == FVOR) {

    if (fm->items[pos].type == INT) {
      if (valueInt < OP->min){
        valueInt = OP->min + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else if (valueInt > OP->max){
        valueInt = OP->max - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else{
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == LONG) {
      if (valueLong < OP->min){
        valueLong = OP->min + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else if (valueLong > OP->max){
        valueLong = OP->max - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else{
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == FLOAT) {
      if (valueFloat < OP->min){
        valueFloat = OP->min + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else if (valueFloat > OP->max){
        valueFloat = OP->max - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else{
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE) {
      if (valueDouble < OP->min){
        valueDouble = OP->min + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else if (valueInt > OP->max){
        valueDouble = OP->max - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else{
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

  }

  if (OP->type == VAT) {

    if (fm->items[pos].type == INT){
      if (valueInt <= OP->threshold){
        valueInt = OP->threshold + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);

      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == LONG){
      if (valueLong <= OP->threshold){
        valueLong = OP->threshold + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE){
      if (valueDouble <= OP->threshold){
        valueDouble = (double)OP->threshold + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == FLOAT) {
      if (valueFloat <= (float)OP->threshold){
        valueFloat = (float)OP->threshold + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else{
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }
  }

  if (OP->type == FVAT) {

    if (fm->items[pos].type == INT){
      if (valueInt > OP->threshold){
        valueInt = OP->threshold - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == LONG){
      if (valueLong > OP->threshold){
        valueLong = OP->threshold - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE){
      if (valueDouble > OP->threshold){
        valueDouble = OP->threshold - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == FLOAT){
      if (valueFloat > OP->threshold){
        valueFloat = OP->threshold - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

  }//FINAL PAR

  if (OP->type == VBT) {

    if (fm->items[pos].type == INT){
      if (valueInt >= OP->threshold){
        valueInt = OP->threshold - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else{
        //value already below threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == LONG) {
      if (valueLong >= OP->threshold){
        valueLong = OP->threshold - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else{
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE) {
      if (valueDouble >= (double)OP->threshold){
        valueDouble = (double)OP->threshold - (double)OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already below threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == FLOAT) {
      if (valueFloat >= (float)OP->threshold){
        valueFloat = (float)OP->threshold - OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else{
        //value already below threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }
  }

  if (OP->type == FVBT) {

    if (fm->items[pos].type == INT){
      if (valueInt < OP->threshold){
        valueInt = OP->threshold + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == LONG){
      if (valueLong < OP->threshold){
        valueLong = OP->threshold + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE){
      if (valueDouble < OP->threshold){
        valueDouble = OP->threshold + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == FLOAT){
      if (valueFloat < OP->threshold){
        valueFloat = OP->threshold + OP->delta;
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
      }
      else {
        //value already above threshold
        _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 0);
      }
      _FAQAS_mutated = 1;
    }

  }//FINAL PAR

  if (OP->type == IV) {

    if (fm->items[pos].type == INT) {
      valueInt = OP->value;
    }

    if (fm->items[pos].type == LONG) {
      valueLong = (long)OP->value;
    }

    if (fm->items[pos].type == DOUBLE) {
      valueDouble = (double)OP->value;
    }

    if (fm->items[pos].type == FLOAT) {
      valueFloat = (float)OP->value;
    }

    _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
    _FAQAS_mutated = 1;
  }

  if (OP->type == SS){

    if (fm->items[pos].type == INT){
      int shift = (int)OP->delta;
      valueInt = (int)valueInt + shift;
    }

    if (fm->items[pos].type == LONG){
      long int shift = (long int)OP->delta;
      valueLong = (long int)valueLong + shift;
    }

    if (fm->items[pos].type == DOUBLE){
      double shift = (double)OP->delta;
      valueDouble = (double)valueDouble + shift;
    }

    if (fm->items[pos].type == FLOAT){
      float shift = (float)OP->delta;
      valueFloat = (float)valueFloat + shift;
    }

    _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
    _FAQAS_mutated = 1;
  }

  if (OP->type == INV) {

    int inv_success = 1;

    if (fm->items[pos].type == INT) {
      int upper = OP->max;
      int lower = OP->min;

      if (upper == lower) {
        valueInt = upper;
      }
      else if (upper < lower) {
        inv_success = 0;
      }

      else {

        int randomNum = valueInt;
        int avoidInfinite = 0;

        while (valueInt == randomNum) {
          randomNum = (rand() % (upper - lower + 1)) + lower;
          avoidInfinite = avoidInfinite + 1;

          if (avoidInfinite == 1000) {
            inv_success = 0;
            break;
          }
        }
        valueInt = randomNum;
      }
    }

    if (fm->items[pos].type == LONG) {
      long int upper = (long int)OP->max;
      long int lower = (long int)OP->min;

      if (upper == lower) {
        valueLong = upper;
        // FIXME: throw a warning
      }
      else if (upper < lower) {
        inv_success = 0;
      }
      else {
        long int randomNum = valueLong;
        int avoidInfinite = 0;

        while (valueLong == randomNum) {
          randomNum = (rand() % (upper - lower + 1)) + lower;
          avoidInfinite = avoidInfinite + 1;

          if (avoidInfinite == 1000) {
            inv_success = 0;
            break;
          }
        }
        valueLong = randomNum;
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
        inv_success = 0;
      }
      else {
        double randomNum = valueDouble;
        int avoidInfinite = 0;

        while (valueDouble == randomNum) {
          randomNum = ((double)rand() * (upper - lower)) / RAND_MAX + lower;
          avoidInfinite = avoidInfinite + 1;

          if (avoidInfinite == 1000) {
            inv_success = 0;
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
        inv_success = 0;
      }
      else {
        float randomNum = valueFloat;
        int avoidInfinite = 0;

        while (valueFloat == randomNum) {
          randomNum = ((float)rand() * (upper - lower)) / RAND_MAX + lower;
          avoidInfinite = avoidInfinite + 1;

          if (avoidInfinite == 1000) {
            inv_success = 0;
            break;
          }
        }
        valueFloat = randomNum;
      }
    }
    _FAQAS_operator_coverage(MUTATION, global_mutation_counter, inv_success);
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
    _FAQAS_operator_coverage(MUTATION, global_mutation_counter, 1);
  }

  if (_FAQAS_mutated != 1) {
    return 0;
  }

  // Store the data

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
    memcpy(&fullNumber, &valueLong, sizeof(valueLong));
    break;
  }

  int counter = 0;
  int stepWrite = 0;

  while (counter < span) {
    stepWrite = 8 * sizeof(data[data_pos + counter]);
    int startSlice = (span - counter - 1) * stepWrite;
    int endSlice = (span - counter) * stepWrite - 1;
    unsigned long long slice =
        _FAQAS_slice_it_up(fullNumber, startSlice, endSlice);
    memcpy(&data[data_pos + counter], &slice, sizeof(data[data_pos + counter]));
    counter = counter + 1;
  }
  global_mutation_counter = global_mutation_counter + 1;
  return _FAQAS_mutated;
}
