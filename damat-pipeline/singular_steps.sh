#DAMAt works in six steps

#step 1 prepare the fm and the test list

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
