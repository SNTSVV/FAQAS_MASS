
#include <iostream>
#include <stdlib.h> 
#include <stdio.h>

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

enum MutationType{
    BF,
    IV,
    VOR,
    VAT,
    VBT,
    INV
};

int _FAQAS_mutated = 0;

struct MutationOperator {
    MutationType type;
    int min;
    int max;
    int delta;
    int state;
};

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

