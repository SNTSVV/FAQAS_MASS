#!/bin/bash

MUTANT_LIST=$1

COV_SCRIPT=$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-IdentifyEquivalentAndRedundantMutants/FAQAS-GenerateCodeCoverageMatrixesEqRed.sh
DIST_SCRIPT=$MASS/FAQAS-CompileAndExecuteMutants/FAQAS-IdentifyEquivalentAndRedundantMutants/distance.py

MUTANTS_RUN=$APP_RUN_DIR/MUTATION/test_runs
RESULTS=$COV_FILES

EXEC_DIR=$APP_RUN_DIR/DETECTION/test_runs
mkdir -p $EXEC_DIR

MUTANT_COVERAGE_FOLDER=$APP_RUN_DIR/DETECTION/MUT_COV_FOLDER
mkdir -p $MUTANT_COVERAGE_FOLDER

TST_COVERAGE_FOLDER=$APP_RUN_DIR/DETECTION/TST_COV_FOLDER
mkdir -p $TST_COVERAGE_FOLDER

pushd $APP_RUN_DIR/COV_FILES
find . -type d -links 2 -exec mkdir -p "$TST_COVERAGE_FOLDER/{}" \;
find . -name '*.c' -exec cp --parents {} $TST_COVERAGE_FOLDER \;
popd

LOGFILE=$EXEC_DIR/main.csv
touch $LOGFILE

shopt -s nullglob;
for i in $(find $MUTANTS_DIR -name '*.c');do

    file_wo_opt=${i//$MUTANTS_DIR/}
    mutant_name="$(basename -- ${file_wo_opt%.*})"
    
    line_number=$(echo $mutant_name | awk -F'.' '{print $3}')
    
    file_wo_mut_end=.${file_wo_opt%%.*}.c

    filename="$(basename -- ${file_wo_mut_end%.*})"

    filename_orig=$(echo $file_wo_mut_end | sed -e "s/\(.*\)$filename\//\1/g" | sed "s:./::")

    if grep -Fxq "${mutant_name}|${filename_orig}" $MUTANT_LIST;then
        start_time=$(($(date +%s%N)/1000000))

        mutant_path=$EXEC_DIR/$mutant_name
        mkdir -p $mutant_path

        MUTANT_LOGFILE=$mutant_path/mutant.log
        touch $MUTANT_LOGFILE

        echo "------------------------------------" 2>&1 | tee -a $MUTANT_LOGFILE
        echo "Mutant: "$i 2>&1 | tee -a $MUTANT_LOGFILE

        # replacing mutant by original source
        cd $PROJ

        echo cp $filename_orig $filename_orig.orig 2>&1 | tee -a $MUTANT_LOGFILE
        cp $filename_orig $filename_orig.orig

        echo cp $i $filename_orig 2>&1 | tee -a $MUTANT_LOGFILE
        cp $i $filename_orig

        tar xzf $MUTANTS_RUN/$mutant_name/coverage.tar.gz

        cd coverage
        for f in *.tar.gz;do tar -xzf "$f";done

        find . -name '*.gc*' -exec cp --parents {} $TST_COVERAGE_FOLDER \;
        cd ../ && rm -rf coverage/

        source $COV_SCRIPT $TST_COVERAGE_FOLDER $filename_orig $mutant_name
        
        cd $TST_COVERAGE_FOLDER

        find . -name 'coverage.txt' -exec cp --parents {} $MUTANT_COVERAGE_FOLDER \;
        pushd $MUTANT_COVERAGE_FOLDER

        for tst in */;do

            echo $tst  2>&1 | tee -a $MUTANT_LOGFILE

            mutant_start_time=$(($(date +%s%N)/1000000))

            RESULTS_COV=$RESULTS/${tst}${GC_FILES_RELATIVE_PATH}/coverage.txt
            RESULTS_NEW_COV=$MUTANT_COVERAGE_FOLDER/${tst}${GC_FILES_RELATIVE_PATH}/coverage.txt

            if ! grep -Fq $filename_orig $RESULTS_COV;then
                continue
            fi

            grep -F $filename_orig $RESULTS_COV  2>&1 | tee -a $MUTANT_LOGFILE
            grep -F $filename_orig $RESULTS_NEW_COV  2>&1 | tee -a $MUTANT_LOGFILE

            echo -n "${filename_orig};${mutant_name};${tst};" >> $LOGFILE

            $DIST_SCRIPT --name "$filename_orig" --cov_a "$RESULTS_COV" --cov_b "$RESULTS_NEW_COV" --result "$LOGFILE" --line "$line_number" | tee -a $MUTANT_LOGFILE

            mutant_end_time=$(($(date +%s%N)/1000000))
            mutant_elapsed="$(($mutant_end_time-$mutant_start_time))"

            echo -ne ";${mutant_elapsed}\n" >> $LOGFILE
        done
        popd

        echo "Replacing original source " 2>&1 | tee -a $MUTANT_LOGFILE
        cd $PROJ
        mv $filename_orig.orig $filename_orig
        touch $filename_orig

        rm -rf $MUTANT_COVERAGE_FOLDER/*

        # reset coverage information
        find $TST_COVERAGE_FOLDER -name '*.gc*' -delete
        find $TST_COVERAGE_FOLDER -name 'coverage.txt' -delete

        end_time=$(($(date +%s%N)/1000000))
        elapsed="$(($end_time-$start_time))"
        echo "elapsed time $elapsed [ms]"  2>&1 | tee -a $MUTANT_LOGFILE

    else
        continue
    fi
done
