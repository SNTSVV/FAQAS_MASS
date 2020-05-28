#include <iostream>
#include <vector>
#include <stdlib.h> 

#define MAX_OPS 10
#define ITEMS 10
//#define MUTATIONOPT 2

int MUTATION=MUTATIONOPT;

enum DataType {
    INT,
    FLOAT,
};

enum MutationType{
    BF,
    IV,
    VOR    
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
    int operatorsN;
    struct MutationOperator operators[MAX_OPS];
};

struct FaultModel {
    int itemsN;
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

struct FaultModel* _FAQAS_GetIfStatus_FM(){
    FaultModel *fm = _FAQAS_create_FM(3);

    //Following code should be automatically generated
    fm->items[0].type=INT;
    fm->items[0].operatorsN=1;
    fm->items[0].operators[0].type=BF;
    fm->items[0].operators[0].min=0;
    fm->items[0].operators[0].max=0;
    
    fm->items[1].type=INT;
    fm->items[1].operatorsN=1;
    fm->items[1].operators[0].type=VOR;
    fm->items[1].operators[0].min=0;
    fm->items[1].operators[0].max=5;
    fm->items[1].operators[0].delta=1;

    fm->items[2].type=INT;
    fm->items[2].operatorsN=1;
    fm->items[2].operators[0].type=BF;
    fm->items[2].operators[0].min=0;
    fm->items[2].operators[0].max=0;

    
    return fm;
}

int selectItem(FaultModel *dm){
	if ( MUTATION == 0 )
		return 0;
	if ( MUTATION == 1 )
		return 1;
	if ( MUTATION == 2 )
		return 1;
	if ( MUTATION == 3 )
		return 2;

	return -1;
}

int selectOperator(FaultModel *dm){
	if ( MUTATION == 0 )
		return 0;
	if ( MUTATION == 1 )
		return 0;
	if ( MUTATION == 2 )
		return 0;
	if ( MUTATION == 3 )
		return 0;

	return -1;
}

int selectOperation(FaultModel *dm){
	if ( MUTATION == 0 )
		return 0;
	if ( MUTATION == 1 )
		return 0;
	if ( MUTATION == 2 )
		return 1;
	if ( MUTATION == 3 )
		return 0;

	return -1;
}

int mutate( std::vector<int> *v, FaultModel *fm ){
    if ( _FAQAS_mutated == 1 )
	return 0;

    if ( MUTATION == -1 )
	return 0;

    int pos = selectItem(fm);
    int op = selectOperator(fm);
    int opt = selectOperation(fm);

    DataItem *item = &(fm->items[pos]);
    MutationOperator *OP = &(fm->items[pos].operators[op]);

    //std::cout << pos << " " << op << " " << opt << '\n'    ;

    //std::cout << OP->type << '\n';

    if ( OP->type == BF ){
        int value = v->data()[pos];
	//FIXME: handle min-max
   	int mask = 1;  //00000011

   	value = value ^ mask; //00000100 
	
	v->data()[pos]=value;
	
	_FAQAS_mutated == 1;

	return 1;
    }
    if ( OP->type == VOR ){
	//FIXME: handle different types
	//

	if ( fm->items[pos].type == INT ){	
        	int value = v->data()[pos];

		if ( opt == 0 ){
			value = OP->min-OP->delta;
		} else if (opt == 1 ){
			value = OP->max+OP->delta;
		} else {
			//ERROR
		}
	
		v->data()[pos]=value;
		
		_FAQAS_mutated == 1;
	}
	return 1;
    }

}

 
int main()
{
    // Create a vector containing integers
    std::vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    FaultModel *fm = _FAQAS_GetIfStatus_FM();
    mutate( &v, fm );    

    for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }
}
