#!/bin/bash

PROJ=$PROJ
TST=$1

PARSER=$MASS/FAQAS-GenerateCodeCoverageMatrixes/FAQAS-UpdateCoverage.sh
COVERAGE_TYPE=0 # 0 means individual, 1 means global

SOURCE=$2
MUTANT=$3

shopt -s extglob

for tst in $TST/*/; do

    pushd $tst/$GC_FILES_RELATIVE_PATH > /dev/null

    GC_FILES_FULL_PATH=$(pwd)

    for gcda in $(find . -name '*.gcda'); do
        output=$(gcov $gcda 2>&1)

        mismatch=$(echo $output | grep "stamp mismatch" | wc -l)
        if [ $mismatch -eq 1 ];then
            echo -ne "${SOURCE};${MUTANT};${tst};NO_COVERAGE_PRODUCED;0\n"
            continue
        fi

        # do not consider gcov files with 0.00% coverage
        covered=0
        for p in $(echo $output | grep -o '[0-9.]*%');do
            percent=$(echo $p | sed 's:\%::')
            if (( $(echo "$percent > 0" | bc -l) ));then
                covered=1
            fi
        done

        if [ $covered -eq 0 ]; then
            gcov_filename=$(echo $output |  awk -F"'" '{print $4}')
            # remove gcov files with 0.00% coverage
            rm $gcov_filename
        else
            test_name=$(echo $tst | sed 's/\.//g; s:\/::g;')

            for gcov_file in $(find . -name '*.gcov'); do
                first_line=$(head -n 1 $gcov_file)
#                full_path=$(echo $first_line | sed -n -e 's/^.*Source://p' | xargs -i realpath "$PROJ"/{})
                
                full_path=$(echo $first_line | sed -n -e 's/^.*Source://p' | sed 's:\.\.\/::g' | xargs -i realpath "$PROJ"/{} 2>&1)
                
                if [ ! -z $COVERAGE_NOT_INCLUDE ];then
                    full_path_filtered=$(echo $full_path | grep "$PROJ" | grep -v "$COVERAGE_NOT_INCLUDE")
                else
                    full_path_filtered=$(echo $full_path | grep "$PROJ")
                fi


                if [ -z "$full_path_filtered" ] || [ ! -f "$full_path_filtered" ];then
                    rm $gcov_file
                    continue
                fi
            
                source_file=$(echo $full_path_filtered | sed 's/.*Data\///')

                if [ $COVERAGE_TYPE -eq 1 ];then
                    source $PARSER $gcov_file $source_file $TST
                else
                    source $PARSER $gcov_file $source_file $GC_FILES_FULL_PATH
                fi

                # not longer useful
                rm $gcov_file
            done
        fi
    done

    popd > /dev/null
done
