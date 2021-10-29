
#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#


# DAMAt

This package implements DaMAT, a data-driven mutation testing tool.
Data-driven mutation testing aims to assess test suites by simulating faults that affect the data produced, received, or exchanged by the software and its components.
The considered faults might be due to programming errors, hardware problems, or critical situations in the environment (e.g., noise in the channel). The data is then automatically  mutated (i.e., modified) by a set of operators that aim to replicate these faults.
The mutation operators are defined in a fault model.
The fault model is produced by software engineers based on their domain knowledge and experience.

## Set-up and Initialization

### Dependencies

	* Python 3.6.8 or higher
	* GNU bash, version 4.2.46 or higher


### Initialization of the DAMAt workspace

All the scripts for the DAMAt pipeline are contained in the DAMAt folder.
All DAMAt steps will take place inside this folder, which shall be placed by the engineer in a path of their choosing.

### Writing a list of all test cases

The user shall provide the list of all the test cases with corresponding nominal time in csv format, using the tests.csv  file as an example.
In the first column of the csv file, there shall be an identifier for the test case. The second column shall contain the nominal execution time in ms.

	test_01,11002
	test_02,13456
	test_03,58347

### Setting variables for the DAMAt pipeline

The user must set the following variables inside the DAMAt_configure.sh script

	* tests_list

	* fault_model

	* buffer_type

	* singleton

	* padding


The variable test_list shall be set to the path of the csv containing the test names and execution times.

The variable fault_model shall be set to the path of the csv containing the fault model of the SUT that the engineer shall define for the SUT.

The variable buffer_type shall contain the type of the elements of the buffer that will be targeted by the mutation.

The variable singleton shall be set to TRUE or FALSE. If set to TRUE, the Fault Model will be initialized in a singleton variable, to avoid memory issues.

The variable padding shall be set to an integer number representing the number of bytes to skip at the beginning of the target buffer. Normally it shall be set to 0, but it can be used to skip the header of the buffer if needed.

### Setting up the compilation of the mutants

The user shall modify DAMAt_compile.sh to include the commands for the compilation of the mutants.
Every mutant is identified by an integer called "MutationOpt".
To enable the data-driven mutation, the engineer shall compile the SUT with this macro enabled:

-DMUTATIONOPT=<MutationOpt>.

To use the singleton mode, the engineer shall compile the SUT with this macro enabled:

  -D_FAQAS_SINGLETON_FM="TRUE".

Once completed, the DAMAt_compile.sh script shall take as input the value of the MutationOpt that refers to the mutant being currently compiled and of the singleton variable, and then compile the SUT accordingly.

### Setting up the execution of the test suite against the mutants

The user shall modify DAMAt_run_tests.sh stub to include the commands for the execution of the mutants.

Every mutant is identified by an integer called "MutationOpt".
This script shall take as input the mutationOpt and a list of tests in the format previously described.

The user shall complete the stub by substituting the generic execution function with a command or a series of commands that execute a specific test case using the test identifier specified in the csv as input.
The user shall use the timeout command as shown in the example included in the stub to set a timeout.

The output of the the new function shall be 0 if the test passes, different from 0 if it fails and 124 in case of timeout.

## Executing DAMAt

DAMAt works in six steps:

### Step 1
The engineer is expected to write a fault model in the csv format with the definition of all the mutation operators they want to apply.

### Step 2

The engineer shall run the following command in the terminal inside the DAMAt_pipeline folder:

	bash DAMAt_probe_generation.sh

This procedure will produce three files:


	* FAQAS_dataDrivenMutator.h: the mutation API.
	* FAQAS_mutants_table.csv: a csv table with the mutationOpt and definition of all the generated mutants.
	* function_calls.out function templates for the mutation probes to insert in the SUT.


a copy of the first two files must remain in the DAMAt folder for the correct execution of the data analysis section of the pipeline.

### Step 3
The engineer will manually instrument the SUT by:


	* copying the FAQAS_dataDrivenMutator.h file in the same folder as the file containing the target function that they chose to instrument.
	* including the mutation API inthe chose file by adding
	#include "FAQAS_dataDrivenMutator.h"

	* inserting function calls in the chosen function based on the prototypes contained in function_calls.out.


An example of a probe is included below, where v is the vector representing the buffer:

	mutate_FM_fault_model_01( &v );


### Step 4, Step 5 and Step 6

The other steps of the DAMAt procedure are carried out automatically by the pipeline.
The pipeline can be started by running the DAMAt_mutants_launcher.sh script with the following command, executed inside the DAMAt_pipeline folder:

bash DAMAt_mutants_launcher.sh

## DAMAt results

After the execution of DAMAt, the results are stored in the .../DAMAt/results folder that will be automatically generated.

The results folder contains files relative to the metrics defined to characterize the results of the full execution of DAMAt:

## Normal Termination

If the engineer decides to interrupt DAMAt execution, it can be done by sending a signal interrupt SIGINT to the running process.

## Recover Runs

If for any reason the execution of DAMAt is interrupted, an engineer can restart the process from a specific task if all preconditions are met, manually executing the rest of the steps.
