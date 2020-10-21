#!/bin/bash

PROJ=/home/mlfs/mlfs
TST=/home/mlfs/unit-reports

PARSER=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/mlfs/update_coverage.sh
TYPE=0 # 1 means global

shopt -s extglob

for tst in $TST/*/; do
	cd $tst/Reports/Coverage/Data	
	coverage_data=`pwd`

	for da in `find . -name '*.gcda'`; do
		output=$(gcov $da 2>&1)

        mismatch=$(echo $output | grep "stamp mismatch" | wc -l)
        if [ $mismatch -eq 1 ];then                                                                                   
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
			test_name=`echo $tst | sed 's/\.//g; s:\/::g;'`
                        
			for g in `find . -name '*.gcov'`; do
				first_line=`head -n 1 $g`
				full_path=`echo $first_line | sed -n -e 's/^.*Source://p' | xargs -i realpath {}`
				
				path=`echo $full_path | sed 's/.*Data\///'`

			if [ $TYPE -eq 1 ];then	
				source $PARSER $g $path $coverage_data $TST
			else
				source $PARSER $g $path $coverage_data
			fi
				rm $g
			done
		fi
	done
	
	cd ../../../../
done
