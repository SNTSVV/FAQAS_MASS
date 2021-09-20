# FAQAS_DataDrivenMutator: DAta-driven Mutation Analysis (DAMA)

This project concern the FAQAS DataDrivenMutator for systems that work with data buffers.

The main executable is python/generateDataMutator.py
It generates a file named "FAQAS_dataDrivenMutator.h" after processing a fault model (e.g., faultModel.csv).
The header and footer of "FAQAS_dataDrivenMutator.h" are specified in DDB_TEMPLATE_footer.c and DDB_TEMPLATE_header.c

#Dependencies

DAMA requires Python 3.

#Testing
Test cases for the data-driven mutator can be run by executing python/runTests.sh, and singular test cases can be executed by running the corresponding bash script in the "test" directory (i.e runTest1.sh)
