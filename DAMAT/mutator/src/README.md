# DaMAT_DataDrivenMutator: Data-driven Mutation Analysis with Tables (DaMAT)

This package implements DaMAT, a data-driven mutation analysis tool for systems that work with data buffers.

DaMAT processes a fault model, in csv format, and through the use of `src/generateDataMutator.py`, it generates a file named `FAQAS_dataDrivenMutator.h`. This file provides an API for all the faults defined in the fault model. At the same time, it provides a list of mutation probes that shall be included manually in the SUT source code.

Detailed information about dependencies, and automated generation of the data mutator are provided in the following.

## DaMAT Dependencies

DaMAT requires Python 3.

## Generation of FAQAS_dataDrivenMutator.h

For the generation of the mutator, run the following command:

`python generateDataMutator.py  "<buffer data type>" <fault model>`

If you need to enable the "SINGLETON" option you must run the following command before generating the mutator:

`export _FAQAS_SINGLETON_FM="TRUE"`

When compiling with gcc, the "SINGLETON" option is strongly advised in order to avoid memory issues.

To avoid mutating a certain number of elements at the beginning of the buffer array, you can set this environmental variable (not mandatory):

`export _FAQAS_INITIAL_PADDING=n`

where _n_ is the number of elements to skip.

In addition to FAQAS_dataDrivenMutator.h the script will generate `FAQAS_mutants_table.csv`, which contains the specifics of all the mutants generated from the fault model, and the mutationOpt, the numerical ID of the mutant.

To generate the mutants, the probes must be manually inserted into the source code of the SUT by calling to the function in the FAQAS_dataDrivenMutator.h corresponding to the chosen fault model.

The function takes as input the buffer to mutate.

`mutate_FM_<FaultModel>(&<buffer>)`

To compile the software under test (SUT) with a specific mutant, compile with the macro `-DMUTATIONOPT` as follows:

`g++ -DMUTATIONOPT=<MutationOpt> <SUT.c> -std=c++11 -g -o <SUT_binary>`

To use the singleton option another compilation macro needs to be defined:

`g++ -D_FAQAS_SINGLETON_FM -DMUTATIONOPT=<MutationOpt> <SUT.c> -std=c++11 -g -o <SUT_binary>`

Before running the mutant, the environmental variable `FAQAS_COVERAGE_FILE` **needs** to be set in order to produce coverage data that will be used to compute the mutation score, it can be done as follows:

`export FAQAS_COVERAGE_FILE=<coverage_file>`

## Testing

Test cases for the data-driven mutator can be run by executing `runTests.sh`.
Instead, singular test cases can be executed by running the corresponding bash script in the "tests" directory (i.e runTest1.sh).

The scripts `runTest\*.sh` provide examples of the usage of DaMAT.
