
#
# Copyright (c) University of Luxembourg 2020.
# Created by Enrico VIGANO, enrico.vigano@uni.lu, SnT, 2021.
#

###############################################################################

This version of DAMAt comes with the additional folder "libparam_set_up"
This is how to set up DAMAt to work with libparam:

	1) substitute "damat_pipeline/libparam_set_up/DAMAt_compile.sh" and "damat-pipeline/libparam_set_up/DAMAt_run_tests.sh" in place of the stubs that you find in the "damat-pipeline" folder.

	2) copy the "damat-pipeline/libparam_set_up/fault_model_param.csv" and "damat-pipeline/libparam_set_up/tests_param.csv" in the main folder.

	3) make sure that the variables in DAMAt_configure.sh are correctly defined. They should be:
		tests_list=$DAMAt_FOLDER/tests_param.csv
		fault_model=$DAMAt_FOLDER/fault_model_param.csv
		buffer_type="unsigned long long int"
		padding=0
		singleton="TRUE"

	4) copy "damat-pipeline/libparam_set_up/wscript" in the "libparam" folder.

	5) the folder to copy the FAQAS_dataDrivenMutator.h, once generated is "libparam/include/gs/param"

###############################################################################



DAMAt

This package implements DaMAT, a data-driven mutation analysis tool for systems that work with data buffers.

Detailed information about DAMAt are provided in the following.

Set-up and Initialization
Dependencies


	* Python 3.6.8 or higher
	* GNU bash, version 4.2.46 or higher


Initialization of the DAMAt workspace

All the scripts for the DAMAt pipeline are contained in the DAMAt folder.
All DAMAt steps will take place inside this folder, which shall be placed by the engineer in a path of their choosing.

The folder shall contain the following structure and files:


	* DAMAt_configure.sh: this script defines the necessary variables for the execution of DAMAt. They shall be set by the engineer.
	* DAMAt_probe_generation.sh: this script sets the variables that are necessary to generate the data mutation API and execute the python script generateDataMutator.py to generate them.
	* DAMAt_mutants_launcher.sh: this script starts the DAMAt pipeline.
	* generateDataMutator.py: this is the script that generates the DAMAt mutation API.
	* DDB_TEMPLATE_header.c and DDB_TEMPLATE_footer.c: these are templates used to generate the DAMAt API by generateDataMutator.py
	* DAMAt_compile.sh: this is a stub of the script used to compile a mutant, which shall be completed by the engineer.
	* DAMAt_run_tests.sh: this is a stub of the script used to run the tests, which shall be completed by the engineer.
	* data_analysis: a folder containing five python scripts used for the generation of the final results:

	  * beautify_results.py: this script renders the raw results from the execution of the tests in a more readable format.
	  * get_coverage.py: this script analizes the results of the fault model coverage.
	  * get_operator_coverage.py: this script analizes the results of the operator coverage.
	  * get_stats.py: this script produces statistics from the mutants' execution.
		* get_final_results.py: this script produces a summary of the execution of DAMAt.

	* pipeline_scripts: a folder containing the four scripts that make up the DAMAt pipeline:

		* DAMAt_obtain_coverage.sh: this script obtains fault model coverage data in order to execute only the tests that cover each mutant.
		* get_mutant_test_list.py: this script produces the list of test against which avery mutant shall be executed.
	  * DAMAt_compile_and_run_mutants.sh: this scripts compile each mutant and run it against the SUT test suite.
		* DAMAt_data_analysis.sh: this script executes all the data analysis steps at the end of the execution of the DAMAt pipeline

	* fault_model.csv: an example of a DAMAt fault model in csv format.
 	* tests.csv : an example of list of test cases and nominal times in csv format.


Writing a list of all test cases
The user shall provide the list of all the test cases with corresponding nominal time in csv format, using the tests.csv  file as an example.
In the first column of the csv file, there shall be an identifier for the test case. The second column shall contain the nominal execution time in ms.


test_01,11002
test_02,13456
test_03,58347


This file will be used by the DAMAt pipeline for two purposes:

	* the nominal execution time will be used to set a timeout for each test case.
	* the whole list will be used to generate a list for each mutant containing only the test cases that cover it.


Setting variables for the DAMAt pipeline


The user must set the following variables inside the DAMAt_configure.sh script

tests_list=$DAMAt_FOLDER/tests.csv

fault_model=$DAMAt_FOLDER/fault_model.csv

buffer_type="unsigned char"

singleton="TRUE"

padding=0



The variable test_list shall be set to the path of the csv containing the test names and execution times.

The variable fault_model shall be set to the path of the csv containing the fault model of the SUT that the engineer shall define for the SUT.

The variable buffer_type shall contain the type of the elements of the buffer that will be targeted by the mutation.

The variable singleton shall be set to TRUE or FALSE. If set to TRUE, the Fault Model will be initialized in a singleton variable, to avoid memory issues.

The variable padding shall be set to an integer number representing the number of bytes to skip at the beginning of the target buffer. Normally it shall be set to 0, but it can be used to skip the header of the buffer if needed.

Setting up the compilation of the mutants

The user shall modify DAMAt_compile.sh to include the commands for the compilation of the mutants.
Every mutant is identified by an integer called "MutationOpt".
To enable the data-driven mutation, the engineer shall compile the SUT with this macro enabled:

-DMUTATIONOPT=<MutationOpt>.

To use the singleton mode, the engineer shall compile the SUT with this macro enabled:

  -D_FAQAS_SINGLETON_FM\="TRUE".

A way to do it is to include the following lines in the SUT makefile:

set(CMAKE_CXX_FLAGS "$CMAKE_CXX_FLAGS -DMUTATIONOPT=$ENVMUTATIONOPT")
# comment the following line if you do not want to use the singleton option.
set(CMAKE_CXX_FLAGS "$CMAKE_CXX_FLAGS -D_FAQAS_SINGLETON_FM=$ENV_FAQAS_SINGLETON_FM")

and then export the corresponding variables in the DAMAt_compile.sh script as shown in the DAMAt_compile.sh stub.

Once completed, the DAMAt_compile.sh script shall take as input the value of the MutationOpt that refers to the mutant being currently compiled and of the singleton variable, and then compile the SUT accordingly.

Setting up the execution of the test suite against the mutants

The user shall modify DAMAt_run_tests.sh stub to include the commands for the execution of the mutants.

Every mutant is identified by an integer called "MutationOpt".
This script shall take as input the mutationOpt and a list of tests in the format previously described.

The user shall complete the stub by substituting the generic execution function with a command or a series of commands that execute a specific test case using the test identifier specified in the csv as input.
The user shall use the timeout command as shown in the example included in the stub to set a timeout.

The output of the the new function shall be 0 if the test passes, different from 0 if it fails and 124 in case of timeout.

Executing DAMAt

DAMAt works in six steps:


	* The user prepares a fault model specification tailored to the SUT.
	* DAMAt generates a mutation API with the functions that modify the data according to the provided fault model.
	* The user manually modifies the SUT by introducing mutation probes (i.e., invocations to the mutation API) into its source code.
	* DAMAt generates and compiles mutants.
	* DAMAt executes the test suite against all the mutants.
	* DAMAt generates mutation analysis results.


Step 1
The engineer is expected to write a fault model in the csv format with the definition of all the mutation operators they want to apply.

FaultModel,DataItem,Span,Type,FaultClass,Min,Max,Threshold,Delta,State,Value
fault_model_01,0,1,BIN,BF,3,3,NA,NA,-1,1
fault_model_01,0,1,BIN,BF,4,4,NA,NA,-1,1
fault_model_01,0,1,BIN,BF,5,7,NA,NA,-1,1
fault_model_02,12,2,DOUBLE,VAT,NA,NA,3.6,0.1,NA,NA
fault_model_02,12,2,DOUBLE,FVAT,NA,NA,3.6,0.1,NA,NA
fault_model_02,14,2,DOUBLE,VAT,NA,NA,33.53,0.01,NA,NA
fault_model_02,14,2,DOUBLE,FVAT,NA,NA,33.53,0.01,NA,NA
fault_model_02,14,2,DOUBLE,VBT,NA,NA,24,1,NA,NA
fault_model_02,14,2,DOUBLE,FVBT,NA,NA,24,1,NA,NA
fault_model_03,0,1,HEX,IV,NA,NA,NA,NA,NA,0x51
fault_model_03,0,1,HEX,IV,NA,NA,NA,NA,NA,0x52
fault_model_03,0,1,HEX,IV,NA,NA,NA,NA,NA,0x53
fault_model_03,0,1,HEX,IV,NA,NA,NA,NA,NA,0x54
fault_model_03,0,1,HEX,IV,NA,NA,NA,NA,NA,0x56


Step 2

In this step, DAMAt generates a mutation API with the functions that modify the data according to the provided fault model.

The engineer shall run the following command in the terminal inside the DAMAt_pipeline folder:

bash DAMAt_probe_generation.sh


This procedure will produce three files:


	* FAQAS_dataDrivenMutator.h: the mutation API.
	* FAQAS_mutants_table.csv: a csv table with the mutationOpt and definition of all the generated mutants.
	* function_calls.out function templates for the mutation probes to insert in the SUT.


a copy of the first two files must remain in the DAMAt folder for the correct execution of the data analysis section of the pipeline.

Step 3
The engineer will manually instrument the SUT by:


	* copying the FAQAS_dataDrivenMutator.h file in the same folder as the file containing the target function that they chose to instrument.
	* including the mutation API inthe chose file by adding
	#include "FAQAS_dataDrivenMutator.h"

	* inserting function calls in the chosen function based on the prototypes contained in function_calls.out.


An example of a probe is included below, where v is the vector representing the buffer:
mutate_FM_fault_model_01( &v );


Step 4, Step 5 and Step 6

The other steps of the DAMAt procedure are carried out automatically by the pipeline.
The pipeline can be started by running the DAMAt_mutants_launcher.sh script with the following command, executed inside the DAMAt_pipeline folder:

bash DAMAt_mutants_launcher.sh


Before running all the generated mutants, a special mutant (MutationOpt=-2) will be executed to gather coverage information.
All the subsequent mutants will only be executed against tests that cover them to save execution time.
The lists of test cases executed against every mutant can be found in the folder .../DAMAt/testlists, which will be automatically generated.

These steps can also be executed separately by manually exporting the relevant variables and then running the pipeline scripts one by one. An example of the necessary commands can be found in the following.

#step 2 generating the API
source DAMAt_configure.sh
bash DAMAt_probe_generation.sh

#step 3 manually instrumenting
cp FAQAS_dataDrivenMutator.h ~/libparam/include/gs/param/FAQAS_dataDrivenMutator.h
vim ~/libparam/wscript
vim ~/libparam/src/csp_service_handler.c

# all the following steps are usually performed by DAMAt_mutants_launcher.sh
#step 4 compile, step 5 execute, step 6 produce the results
source DAMAt_configure.sh
bash $PIPELINE_FOLDER/DAMAt_obtain_coverage.sh $DAMAt_FOLDER
bash $PIPELINE_FOLDER/DAMAt_compile_and_run_mutants.sh $DAMAt_FOLDER
bash $PIPELINE_FOLDER/DAMAt_data_analysis.sh $DAMAt_FOLDER


DAMAt results

After the execution of DAMAt, the results are stored in the .../DAMAt/results folder that will be automatically generated.
For every mutant the pipeline will create a subfolder called run_<mutationOpt> containing:

	* A main.csv file with the results of the mutant's execution against the test suite
	* A coverage_<test case>.csv file containing the raw operator coverage data for that mutant-test case couple.
	* A readable_coverage_<test case>.csv file containing the readable operator coverage data for that mutant-test case couple.
	* A <mutationOpt>_exectuion.out file containing an execution log for the mutant.


In addition to that, a logs folder shall be created, containing the full compilation and execution logs for every mutant.

The .../DAMAt/results folder contains also files relative to the metrics defined to characterize the results of the full execution of DAMAt:


	1) Fault model coverage is the percentage of fault models covered by the test suite.
	2) Mutation operation coverage is the percentage of data items that have been mutated at least once, considering only those that belong to the data buffers covered by the test suite.
	3) The mutation score (MS) is the percentage of mutants killed by the test suite (i.e., leading to at least one test case failure) among the mutants that target a fault model and for which at least one mutation operation was successfully performed.


These metrics measure the frequency of the following scenarios:

	1) the message type targeted by a mutant is never exercised.
	2) the message type is covered by the test suite but it is not possible to perform some of the mutation operations.
	3) the mutation is performed but the test suite does not fail.


The file generated by the final steps are:


	* mutation_sum_up.csv: a file containing the previously described three metrics.
	* final_mutants_table.csv: a file containing the definition and status of every mutant.
	* mutation_score_by_data_item.csv: a file containing the mutation score by data item.
	* mutation_score_by_fault_class.csv: a file containing the mutation score by fault class.
	* mutation_score_by_fault_model.csv: a file containing the mutation score by fault model.
	* test_coverage.csv: a file containing the tests covering the different fault models.
	* readable_data.csv: a file containing a more readable version of the execution data.
	* raw_data.csv and raw_data_sorted.csv: these files contain all the execution data.
	* readable_operator_coverage.csv: a file containing a more readable version of the operator coverage data.
	* operator_coverage.csv: a file containing a raw version of the operator coverage data.
	* readable_FM_coverage.csv: a file containing a more readable version of the fault model coverage data.
	* FM_coverage.csv: a file containing a raw version of the fault model coverage data.


Normal Termination

If the engineer decides to interrupt DAMAt execution, it can be done by sending a signal interrupt SIGINT to the running process.

Recover Runs

If for any reason the execution of DAMAt is interrupted, an engineer can restart the process from a specific task if all preconditions are met, manually executing the rest of the steps.
