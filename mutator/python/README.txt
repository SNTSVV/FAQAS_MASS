To generate FAQAS_dataDrivenMutator.h run this command:
python generateDataMutator.py  "<buffer data type>" <fault model>

If you need to enable the "SINGLETON" option you must run the following command before it.

export _FAQAS_SINGLETON_FM="TRUE"

some examples:

ESAIL-ADCS (with the "SINGLETON" option)

export _FAQAS_SINGLETON_FM="TRUE" && python generateDataMutator.py  "unsigned char" fault_model_ESAIL_ADCS.csv

LIBPARAM (without the "SINGLETON" option)

python generateDataMutator.py  "unsigned long long int" LibParamFaultModel.csv

This script should generate two files: FAQAS_dataDrivenMutator.h and FAQAS_mutants_table.csv
