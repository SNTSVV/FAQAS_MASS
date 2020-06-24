# FAQAS_DataDrivenMutator

This project describes the FAQAS DataDrivenMutator to execute data-driven mutaton of systems that work with buffers.

The main executable is mutator/python/generateDataMutator.py
It generates a file named "FAQAS_dataDrivenMutator.h" after processing a fault model (e.g., faultModel.csv).
The header and footer of "FAQAS_dataDrivenMutator.h" are specified in DDB_TEMPLATE_footer.c and DDB_TEMPLATE_header.c

Test cases for the data-driven mutator can be run by executing mutator/python/runTests.sh

Folder mutator/python/examples/esail_integration.2020.06.10/ contains an example of how the dataDrivenMutator can be integrated into the ESAIL system.

