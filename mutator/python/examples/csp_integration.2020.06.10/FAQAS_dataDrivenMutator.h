//max MUTATIONOPT=4
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

typedef enum DataType DataType;

enum MutationType{
    BF,
    IV,
    VOR,
    VAT,
    VBT,
    INV
};

typedef enum MutationType MutationType;

int _FAQAS_mutated = 0;

struct MutationOperator {
    MutationType type;
    int min;
    int max;
    int delta;
    int state;
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

#define SIZE_Identifier 5


struct FaultModel* _FAQAS_Identifier_FM(){
FaultModel *fm = _FAQAS_create_FM(SIZE_Identifier);
fm->ID = 0;

fm->items[0].operators[0].type=BF;
fm->items[0].operators[0].min=0;
fm->items[0].operators[0].max=0;
fm->items[0].operators[0].state=-1;
fm->items[0].operatorsN=1;
fm->items[0].span=1;
fm->items[0].type=BIN;

fm->items[1].operators[0].type=VOR;
fm->items[1].operators[0].min=0;
fm->items[1].operators[0].max=5;
fm->items[1].operators[0].delta=1;
fm->items[1].operatorsN=1;
fm->items[1].span=1;
fm->items[1].type=INT;

fm->items[2].operators[0].type=BF;
fm->items[2].operators[0].min=0;
fm->items[2].operators[0].max=0;
fm->items[2].operators[0].state=-1;
fm->items[2].operatorsN=1;
fm->items[2].span=2;
fm->items[2].type=BIN;

fm->items[4].operators[0].type=BF;
fm->items[4].operators[0].min=0;
fm->items[4].operators[0].max=0;
fm->items[4].operators[0].state=-1;
fm->items[4].operatorsN=1;
fm->items[4].span=1;
fm->items[4].type=BIN;
return fm;
}


int _FAQAS_selectItem(){
if ( MUTATION == 0 )
    return 0;
if ( MUTATION == 1 )
    return 1;
if ( MUTATION == 2 )
    return 1;
if ( MUTATION == 3 )
    return 2;
if ( MUTATION == 4 )
    return 4;
return -999;
}
int _FAQAS_selectOperator(){
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
return -999;
}
int _FAQAS_selectOperation(){
if ( MUTATION == 0 )
    return 0;
if ( MUTATION == 1 )
    return 0;
if ( MUTATION == 2 )
    return 1;
if ( MUTATION == 3 )
    return 0;
if ( MUTATION == 4 )
    return 0;
return -999;
}

int _FAQAS_mutate( unsigned int *data, FaultModel *fm ){
    if ( _FAQAS_mutated == 1 )
	return 0;

    if ( MUTATION == -1 )
	return 0;

    if ( MUTATION == -2 ) {
        FILE *f = fopen ("/home/csp/logging.txt", "ab+");
        fprintf(f, "fm.ID: %d\n", fm->ID);
        fclose(f);

        return 0;
    }
    
    int pos = _FAQAS_selectItem();
    int op = _FAQAS_selectOperator();
    int opt = _FAQAS_selectOperation();

    //FIXME: handle items spanning over multiple array positions
    int valueInt = -999;
    int valueBin = -999;
    double valueDouble = -999;

    //
    //Load the data
    //
    if ( fm->items[pos].type == BIN ){
        valueBin = (int) data[pos];
    }
    if ( fm->items[pos].type == INT ){
        valueInt = (int) data[pos];
    }
    if ( fm->items[pos].type == DOUBLE ){
        valueDouble = (double) data[pos];
    }

    MutationOperator *OP = &(fm->items[pos].operators[op]);

    if ( OP->type == BF ){
    	//FIXME: handle min-max
   	    int mask = 1;  //00000011

   	    valueBin = valueBin ^ mask; //00000100 
	
	    _FAQAS_mutated = 1;

    }

    if ( OP->type == VOR ){
	//FIXME: handle different types
	//

    	if ( fm->items[pos].type == INT ){	

	    	if ( opt == 0 ){
		    	valueInt = OP->min-OP->delta;
		    } else if (opt == 1 ){
			    valueInt = OP->max+OP->delta;
		    } else {
			    //ERROR
		    }
		
		    _FAQAS_mutated = 1;
	    }
	    
    }

    if ( _FAQAS_mutated != 1 ){
        return 0;
    }
    
    //
    //Store the data
    //
    //FIXME: handle span
    if ( fm->items[pos].type == INT ){
        data[pos] = valueInt;
    }
    if ( fm->items[pos].type == DOUBLE ){
        data[pos] = valueDouble;
    }
    if ( fm->items[pos].type == BIN ){
        data[pos] = valueBin;
    }

    return _FAQAS_mutated;
}

