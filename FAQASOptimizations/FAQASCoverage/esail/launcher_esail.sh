#/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

GCOV=/opt/rtems-4.8-SAT-AIS/bin/sparc-rtems4.8-gcov

PROJ=/home/svf/Obsw/Source
TST=/home/svf/mutant_coverage

PARSER=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/esail/update_coverage.sh

SOURCE=$1
MUTANT=$2

GCOV_FILE_PATH=$(dirname -- "$SOURCE")

shopt -s extglob
for tst in $TST/*/;do
#        echo "$SOURCE test $tst" 

        coverage_file=$tst/coverage.txt
        touch -a $coverage_file

        cp $tst/*.gc* $PROJ/$GCOV_FILE_PATH

        pushd $PROJ/$GCOV_FILE_PATH

        for da in $(find . -name '*.gcda'); do
            output=$($GCOV $da 2>&1)

            mismatch=$(echo $output | grep "stamp mismatch" | wc -l)
            if [ $mismatch -eq 1 ];then
                echo -ne "${SOURCE};${MUTANT};${d};NO_COVERAGE_PRODUCED;0\n"
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
                 gcov_filename=`echo $output |  awk -F"'" '{print $4}'`
                 rm $gcov_filename
            else
                for g in $(find . -name '*.gcov');do
                    first_line=$(head -n 1 $g)
                    path=$(echo $first_line | sed -n -e 's/^.*Source://p')

                    if [[ $path == *"/opt/rtems-4.8-SAT-AIS"* ]];then
                        rm $g
                        continue
                    fi
                    source $PARSER $g $path $coverage_file
                    rm $g
                done
            fi
        done

        rm $PROJ/$GCOV_FILE_PATH/*.gc*

        popd
done
