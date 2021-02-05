//
// Copyright (c) University of Luxembourg 2020.
// Created by Fabrizio PASTORE, fabrizio.pastore@uni.lu, SnT, 2020.
// Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
//

int _FAQAS_mutate(BUFFER_TYPE *data, FaultModel *fm)
{
  if (_FAQAS_mutated == 1)
    return 0;

  if (MUTATION == -1)
    return 0;

  if (MUTATION == -2)
  {
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
  float valueFloat;


  //
  // Load the data
  //
  if (fm->items[pos].type == BIN)
  {
    valueBin = (int)data[pos];
  }
  if (fm->items[pos].type == INT)
  {
    valueInt = (int)data[pos];
  }
  if (fm->items[pos].type == DOUBLE)
  {
    valueDouble = (double)data[pos];
  }
  if (fm->items[pos].type == FLOAT)
  {
    valueFloat = (float)data[pos];
  }

  MutationOperator *OP = &(fm->items[pos].operators[op]);

  if (OP->type == BF)
  {
    // FIXME: handle min-max
    int mask = 1; // 00000011

    valueBin = valueBin ^ mask; // 00000100

    _FAQAS_mutated = 1;
  }

  if (OP->type == VOR)
  {
    // FIXME: handle different types
    //

    if (fm->items[pos].type == INT)
    {

      if (opt == 0)
      {

        valueInt = OP->min - OP->delta;
      }

      else if (opt == 1)
      {
        valueInt = OP->max + OP->delta;
      }

      else
      {
        // ERROR
      }

      _FAQAS_mutated = 1;
    }

    if (fm->items[pos].type == DOUBLE)
    {

      if (opt == 0)
      {

        valueDouble = (double) OP->min - OP->delta;
      }

      else if (opt == 1)
      {
        valueDouble = (double) OP->max + OP->delta;
      }

      else
      {
        // ERROR
      }

      _FAQAS_mutated = 1;
    }


    if (fm->items[pos].type == FLOAT)
    {

      if (opt == 0)
      {

        valueFloat = (float) OP->min - OP->delta;
      }

      else if (opt == 1)
      {
        valueFloat = (float) OP->max + OP->delta;
      }

      else
      {
        // ERROR
      }

      _FAQAS_mutated = 1;
    }

  }

  if (OP->type == VAT)
  {
    // FIXME: handle different types
    //

    if (fm->items[pos].type == INT)
    {

      valueInt = OP->threshold + OP->delta;

      _FAQAS_mutated = 1;

    }

  if (fm->items[pos].type == DOUBLE)
  {

    valueDouble = (double) OP->threshold + OP->delta;

    _FAQAS_mutated = 1;

  }

  if (fm->items[pos].type == FLOAT)
  {

    valueDouble = (float) OP->threshold + OP->delta;

    _FAQAS_mutated = 1;
  }


}

// if (OP->type == VBT)
// {
//   // FIXME: handle different types
//   //
//
//   if (fm->items[pos].type == INT)
//   {
//
//     valueInt = OP->threshold - OP->delta;
//
//     _FAQAS_mutated = 1;
//   }
//
//   if (fm->items[pos].type == DOUBLE)
//   {
//
//     valueDouble =  (double) OP->threshold - OP->delta;
//
//     _FAQAS_mutated = 1;
//   }
//
//   if (fm->items[pos].type == FLOAT)
//   {
//
//     valueFloat =  (float) OP->threshold - OP->delta;
//
//     _FAQAS_mutated = 1;
//   }
//
//
// }

if (OP->type == IV)
{
  // FIXME: handle different types
  //
  if (fm->items[pos].type == INT)
  {

    valueInt = OP->value;
  }

  if (fm->items[pos].type == DOUBLE)
  {

    valueDouble = (double) OP->value;
  }

  if (fm->items[pos].type == FLOAT)
  {

    valueFloat = (float) OP->value;
  }

  _FAQAS_mutated = 1;
}

if (OP->type == VBT)
{
  // FIXME: handle different types
  //

  if (fm->items[pos].type == INT)
  {

    int limit = OP->threshold;
    int shift = OP->delta;

    if (valueInt >= limit)
    {
      valueInt = valueInt - shift;
    }
    else
    {
      valueInt = valueInt + shift;
    }
  }

  if (fm->items[pos].type == DOUBLE)
  {

    double limit = OP->threshold;
    double shift = OP->delta;

    if (valueInt >= limit)
    {
      valueDouble =  (double) valueDouble - shift;
    }
    else
    {
      valueDouble =  (double) valueDouble + shift;
    }
  }

  if (fm->items[pos].type == FLOAT)
  {

    float limit = OP->threshold;
    float shift = OP->delta;

    if (valueInt >= limit)
    {
      valueDouble =  (float) valueDouble - shift;
    }
    else
    {
      valueFloat =  (float) valueDouble + shift;
    }
  }

  _FAQAS_mutated = 1;
}

if (OP->type == INV)
{
  // FIXME: handle different types
  //
  if (fm->items[pos].type == INT)
  {

    int upper = OP->max;
    int lower = OP->min;

    if (upper == lower)
    {
      valueInt = upper;
      // FIXME: throw a warning
    }

    else if (upper<lower)
    {
      //FIXME: throw an error
    }

    else
    {
      srand(time(0));
      int randomNum = valueInt;
      int avoidInfinite = 0;

      while (valueInt == randomNum)
      {

        randomNum = (rand() % (upper - lower + 1)) + lower;
        avoidInfinite = avoidInfinite + 1;

        if (avoidInfinite == 1000)
        {
          randomNum = upper;
          break;
        }
      }
      valueInt = randomNum;
    }
  }

  if (fm->items[pos].type == DOUBLE)
  {

    double upper = OP->max;
    double lower = OP->min;

    if (upper == lower)
    {
      valueDouble = upper;
      // FIXME: throw a warning
    }

    else if (upper<lower)
    {
      //FIXME: throw an error
    }

    else
    {
      srand(time(0));
      double randomNum = valueDouble;
      int avoidInfinite = 0;

      while (valueDouble == randomNum)
      {


        randomNum = ( (double)rand() * ( upper - lower ) ) / (double)RAND_MAX + lower;

        avoidInfinite = avoidInfinite + 1;

        if (avoidInfinite == 1000)
        {
          randomNum = upper;
          break;
        }
      }
      valueDouble = randomNum;
    }
  }

  if (fm->items[pos].type == FLOAT)
  {

    float upper = OP->max;
    float lower = OP->min;

    if (upper == lower)
    {
      valueFloat = upper;
      // FIXME: throw a warning
    }

    else if (upper<lower)
    {
      //FIXME: throw an error
    }

    else
    {
      srand(time(0));
      float randomNum = valueFloat;
      int avoidInfinite = 0;

      while (valueFloat == randomNum)
      {
        randomNum = ( (float)rand() * ( upper - lower ) ) / (float)RAND_MAX + lower;

        avoidInfinite = avoidInfinite + 1;

        if (avoidInfinite == 1000)
        {
          randomNum = upper;
          break;
        }
      }
      valueDouble = randomNum;
    }
  }


  _FAQAS_mutated = 1;
}




if (_FAQAS_mutated != 1)
{
  return 0;
}

//
// Store the data
//
// FIXME: handle span
if (fm->items[pos].type == INT)
{
  data[pos] = valueInt;
}
if (fm->items[pos].type == DOUBLE)
{
  data[pos] = valueDouble;
}
if (fm->items[pos].type == BIN)
{
  data[pos] = valueBin;
}

return _FAQAS_mutated;
}
