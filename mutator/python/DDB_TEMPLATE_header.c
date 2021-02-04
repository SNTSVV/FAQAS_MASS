//
// Copyright (c) University of Luxembourg 2020.
// Created by Fabrizio PASTORE, fabrizio.pastore@uni.lu, SnT, 2020.
// Modified by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2020.
//

#include <stdlib.h>
#include <stdio.h>

//time.h is  included for seeding the INV random number generator in the footer
#include <time.h>

#define MAX_OPS 10
#define ITEMS 10
//#define MUTATIONOPT 2

int MUTATION=MUTATIONOPT;

enum DataType {
    INT,
    FLOAT,
    DOUBLE,
    BIN
};

typedef enum DataType DataType;

enum MutationType{
    BF,
    IV,
    VOR,
    VAT,
    VBT,
    INV,
    SS
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
