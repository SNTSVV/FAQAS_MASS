#
# Copyright (c) University of Luxembourg 2021.
# Created by Oscar Eduardo CORNEJO OLIVARES, oscar.cornejo@uni.lu, SnT, 2021.
#

# implementation of the bash function run_tst_case
# run_tst_case receives the test case name, executes the test case,
# and returns 0 if the test case passes, otherwise returns 1
#
# example of the MLFS library
#run_tst_case() {
#
#    tst_name=$1
#    tst=$PROJ_TST/$tst_name.xml
#
#    echo $tst_name $tst
#
#    # run the test case
#    cd /home/mlfs/blts_workspace
#    $HOME/blts_install/bin/blts_app -gcrx $tst_name -b coverage --nocsv -s $tst
#
#    # define if test case execution passed or failed
#    summaryreport=$tst_name/Reports/SessionSummaryReport.xml
#    originalreport=$HOME/unit-reports/$summaryreport
#
#    test_cases_failed=`xmllint --xpath "//report_summary/test_set_summary/test_cases_failed/text()" $summaryreport`
#    o_test_cases_failed=`xmllint --xpath "//report_summary/test_set_summary/test_cases_failed/text()" $originalreport`
#
#    echo "comparing with original execution" 2>&1 | tee -a $MUTANT_LOGFILE
#    echo $test_cases_failed $o_test_cases_failed 2>&1 | tee -a $MUTANT_LOGFILE
#
#    if [ "$test_cases_failed" != "$o_test_cases_failed" ]; then
#        return 1
#    else
#        return 0
#    fi
#}
run_tst_case() {

}
