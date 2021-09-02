To generate FAQAS_dataDrivenMutator.h run this command:

python generateDataMutator.py  "<buffer data type>" <fault model>

If you need to enable the "SINGLETON" option you must run the following command before it.

To compile with gcc the "SINGLETON" option is strongly advised in order to avoid memory issues.

export _FAQAS_SINGLETON_FM="TRUE"

To avoid mutating a certain number of elements at the beginning of the buffer array, you can set this environmental variable

export _FAQAS_INITIAL_PADDING=n

where n is the number of elements to skip.


some examples:

ESAIL-ADCS (with the "SINGLETON" option and skipping the first two elements)

export _FAQAS_INITIAL_PADDING=2  _FAQAS_SINGLETON_FM="TRUE" && python generateDataMutator.py  "unsigned char" "./case_studies_fault_models/fault_model_ESAIL_ADCS.csv"

LIBPARAM (without the "SINGLETON" option)

python generateDataMutator.py  "unsigned long long int" "./case_studies_fault_models/fault_model_LibParam.csv"

LIBPARAM (with the "SINGLETON" option)

export _FAQAS_SINGLETON_FM="TRUE" && python generateDataMutator.py  "unsigned long long int" "./case_studies_fault_models/LibParamFaultModel.csv"

This script should generate two files: FAQAS_dataDrivenMutator.h and FAQAS_mutants_table.csv


WORK IN PROGRESS:

for the ESAIL gps

export _FAQAS_SINGLETON_FM="TRUE" && python generateDataMutator.py  "double" "./case_studies_fault_models/fault_model_ESAIL_GPS_fix.csv"

for the ESAIL phdu

export _FAQAS_SINGLETON_FM="TRUE" && python generateDataMutator.py  "unsigned char" "./case_studies_fault_models/fault_model_ESAIL_PDHU_new.csv"
