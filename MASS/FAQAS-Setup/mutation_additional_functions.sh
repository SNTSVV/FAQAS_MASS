
# implementation of the bash function backup_tst_coverage
# backup_tst_coverage compress the coverage data
# of the execution of a test case into a tar.gz file and
# stores it into the mutant folder
#
# this function receives three parameters:
# $1: name of the test case
# $2: name of the source code under analysis (without the extension)
# $3: mutation testing path
#
# example for the MLFS library
#backup_tst_coverage() {
#    tst=$1
#    mutant_src_name=$2
#    mutant_exec_path=$3
#
#    cd $HOME/blts_workspace
#    rm -rf $tst/Reports/Data/libm
#    GZIP=-9 tar czf ${tst}.tar.gz $tst/Reports/Coverage/Data/${mutant_src_name}.gc*
#    mv ${tst}.tar.gz $mutant_exec_path/coverage
#}

backup_tst_coverage() {

}

# implementatino of the bash function run_tst_case
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
