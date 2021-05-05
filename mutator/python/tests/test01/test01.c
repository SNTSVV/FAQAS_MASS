#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <bitset>
#include "FAQAS_dataDrivenMutator.h"


int mutate( std::vector<int> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}

void mutate2( std::vector<unsigned char> *v ){

std::fill(v->begin(), v->end(), 0);

}

void log_data_before( std::vector<unsigned char> *v ){

std::ofstream MyFile("logging.txt", std::ios_base::app);

  MyFile << "\n BEFORE:\n";

for (auto i = v->begin(); i != v->end(); ++i){

  MyFile << std::bitset<8>(*i)  << std::endl;

}

  MyFile << "\n=========\n";
  MyFile.close();

}

void log_data_after( std::vector<unsigned char> *v ){

std::ofstream MyFile("logging.txt", std::ios_base::app);

  MyFile << "\n AFTER:\n";

for (auto i = v->begin(); i != v->end(); ++i){

  MyFile << std::bitset<8>(*i)  << std::endl;

}

  MyFile << "\n=========\n";
  MyFile.close();

}


int main()
{
    // Create a vector containing integers
    std::vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);

    std::vector<unsigned char> g;

    g.push_back(65);
    g.push_back(1);
    g.push_back(1);
    g.push_back(1);
    g.push_back(1);


    log_data_before(&g);
    //MANUALLY ADDED PROBE
    FaultModel *fm = _FAQAS_IfHK_FM();
    mutate( &v, fm );
    mutate2( &g);
    _FAQAS_delete_FM(fm);
    //MANUALLY ADDED PROBE END

    log_data_after(&g);

    for(std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    	std::cout << *it << '\n';
    }



    return 0;
}
