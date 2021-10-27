#!/bin/bash

FAQAS_SEMU_CASE_STUDY_TOPDIR=../

FAQAS_SEMU_CASE_STUDY_WORKSPACE=$FAQAS_SEMU_CASE_STUDY_TOPDIR/WORKSPACE

FAQAS_SEMU_OUTPUT_TOPDIR=$FAQAS_SEMU_CASE_STUDY_WORKSPACE/OUTPUT

FAQAS_SEMU_ANALYSIS_REPORT=$(realpath $FAQAS_SEMU_OUTPUT_TOPDIR/AnalysisReport.csv)

containsElement () {
    local e match="$1"
    shift
    for e; do [[ "$e" == "$match" ]] && return 0; done
    return 1
}


cd $FAQAS_SEMU_OUTPUT_TOPDIR

global_total_mutants=0
global_total_killed_mutants=0
global_total_live_mutants=0

TEMP_REPORT=temp_report.csv

touch $TEMP_REPORT

for f in `find . -name '*.MetaMu.mutant_mapping.json'`
do
    metamu_file=$(basename $f | awk -F'.' '{print $1}')
    
    total_mutants_file=$(jq length $f)
    global_total_mutants=$((global_total_mutants+total_mutants_file))
    
    killed_mutants=()
    for ktest in `find . -name '*.ktest.c' | xargs grep -l ${metamu_file}.c`
    do
        killed_mutants+=($(sed '3q;d' $ktest | cut -d "[" -f2 | cut -d "]" -f1))
    done
    
    total_killed_mutants=$(printf "%s\n" "${killed_mutants[@]}" | sort | uniq | wc -l)
    total_live_mutants=$((total_mutants_file-total_killed_mutants))
    
    global_total_killed_mutants=$((global_total_killed_mutants+total_killed_mutants))
    global_total_live_mutants=$((global_total_live_mutants+total_live_mutants))

    mutant_counter=1
    jq -r -c '.[]' $f | while read mutant
    do
        containsElement $mutant_counter "${killed_mutants[@]}"
        [ $? -eq 0 ] && mutant_status="KILLED" || mutant_status="LIVE"

        echo "$mutant;$mutant_status" >> $TEMP_REPORT
        mutant_counter=$((mutant_counter+1))
    done
    
done

test -f $FAQAS_SEMU_ANALYSIS_REPORT && >$FAQAS_SEMU_ANALYSIS_REPORT || touch $FAQAS_SEMU_ANALYSIS_REPORT

echo "Number of analyzed mutants: "$global_total_mutants >> $FAQAS_SEMU_ANALYSIS_REPORT
echo "Number of killed mutants: "$global_total_killed_mutants >> $FAQAS_SEMU_ANALYSIS_REPORT
echo "Number of live mutants: "$global_total_live_mutants >> $FAQAS_SEMU_ANALYSIS_REPORT

cat $TEMP_REPORT >> $FAQAS_SEMU_ANALYSIS_REPORT && rm $TEMP_REPORT


