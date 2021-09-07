#To generate DAMAt_dataDrivenMutator.h run this command:

python generateDataMutator.py  "<buffer data type>" <fault model>

If you need to enable the "SINGLETON" option you must run the following command before it.

export _DAMAt_SINGLETON_FM="TRUE"

When compiling with gcc, the "SINGLETON" option is strongly advised in order to avoid memory issues.

#To avoid mutating a certain number of elements at the beginning of the buffer array, you can set this environmental variable

export _DAMAt_INITIAL_PADDING=n

where n is the number of elements to skip.

In addition to DAMAt_dataDrivenMutator.h the script will generate DAMAt_mutants_table.csv, which contains the specifics of all the mutants generated from the fault model, and the mutationOpt, the numerical ID of the mutant.

#To generate the mutants, the probes must be manually inserted into the source code of the SUT by calling to the function in the DAMAt_dataDrivenMutator.h corresponding to the chosen fault model.

The function takes as input the buffer to mutate.

mutate_FM_<FaultModel>(&<buffer>)

#To generate a specific mutant, compile with the macro -DMUTATIONOPT as follows:

g++ -DMUTATIONOPT=<MutationOpt> <SUT.c> -std=c++11 -g -o <SUT_binary>
To use the singleton option another compilation macro needs to be defined:

g++ -D_DAMAt_SINGLETON_FM -DMUTATIONOPT=<MutationOpt> <SUT.c> -std=c++11 -g -o <SUT_binary>

Before running the mutant, the environmental variable DAMAt_COVERAGE_FILE
needs to be set in order to produce coverage data that will be used to compute the mutation score.

export DAMAt_COVERAGE_FILE=<coverage_file>
