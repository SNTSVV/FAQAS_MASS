#!/bin/bash

LIBUTIL=/home/gsl/libutil

SRC_MUTANTS=/opt/mutations/src-mutants

EXEC_NAME=test_run_$(date +"%Y%m%d%H%M")
EXEC_DIR=/home/gsl/test_runs/$EXEC_NAME

LOGFILE=$EXEC_DIR/$EXEC_NAME.log
mkdir -p $EXEC_DIR
touch $LOGFILE

shopt -s extglob
trap "exit" INT


if [ ! -f "$LIBUTIL/build/sha512sum" ]; then
    #get hash from original version
	cd $LIBUTIL
	./waf clean
	./waf build
	sha512sum -b $LIBUTIL/build/libgsutil_cmocka.a | sed 's/ .*//' > $LIBUTIL/build/sha512sum	
fi

orig_hash=`cat $LIBUTIL/build/sha512sum`

echo original hash $orig_hash

count=0

for i in $(find $SRC_MUTANTS -name '*.c'); do
	start_time=$(($(date +%s%N)/1000000))

	file_wo_opt=${i//$SRC_MUTANTS/}
	mutant_name="$(basename -- ${file_wo_opt%.*})"
	file_wo_mut_end=.${file_wo_opt%%.*}.c

	filename="$(basename -- ${file_wo_mut_end%.*})"
	filename_orig=$(echo $file_wo_mut_end | sed -e "s/\(.*\)$filename\//\1/g")

	mutant_path=$EXEC_DIR/$mutant_name
	mkdir -p $mutant_path

	MUTANT_LOGFILE=$mutant_path/logfile

	touch $MUTANT_LOGFILE

	echo "------------------------------------" 2>&1 | tee -a $MUTANT_LOGFILE
	echo "Mutant: "$i 2>&1 | tee -a $MUTANT_LOGFILE

	cd $LIBUTIL/src

	# replacing mutant by original source
	echo cp $filename_orig $filename_orig.orig 2>&1 | tee -a $MUTANT_LOGFILE
	cp $filename_orig $filename_orig.orig

	echo cp $i $filename_orig 2>&1 | tee -a $MUTANT_LOGFILE
	cp $i $filename_orig

	cd $LIBUTIL

	# file to contain compilation results
	compilation_results=$i.compilation
	touch $compilation_results
	
	./waf build  2>&1 | tee -a $MUTANT_LOGFILE
	RET_CODE=${PIPESTATUS[0]}	
	
	if [ $RET_CODE -eq 1 ]; then
		echo "Error: mutant could not be compiled" 2>&1 | tee -a $MUTANT_LOGFILE
		echo $mutant_name"      not compiled" 2>&1 | tee -a $LOGFILE
		
		mv $compilation_results $i.notcompiled
	else
		echo "Success: mutant compiled" 2>&1 | tee -a $MUTANT_LOGFILE
        echo $mutant_name"      compiled" 2>&1 | tee -a $LOGFILE	
		
		echo 1 > $compilation_results

		hash=`sha512sum -b $LIBUTIL/build/libgsutil_cmocka.a | sed 's/ .*//'`

		echo "$hash" 2>&1 | tee -a $LOGFILE $MUTANT_LOGFILE	
		echo $hash >> $compilation_results

		if [[ "$hash" != "$orig_hash" ]];then
			echo "$mutant_name not equivalent with original" 2>&1 | tee -a $MUTANT_LOGFILE $LOGFILE 	
			
			redundant=0
			for m in `find $SRC_MUTANTS -name '*.compilation'`;do
				if [[ "$m" == "${i}.compilation" ]];then
					continue
				fi
				m_hash=`sed '2q;d' $m`
				if [[ "$hash" == "$m_hash" ]];then
					echo "$mutant_name redundant with $m" 2>&1 | tee -a $MUTANT_LOGFILE $LOGFILE
					mv $compilation_results $i.redundant
					redundant=1

					break
				fi
			done

			if [ $redundant -eq 0 ];then
				echo "$mutant_name not redundant" 2>&1 | tee -a $MUTANT_LOGFILE $LOGFILE
			fi

		else
			echo "$mutant_name equivalent with original" 2>&1 | tee -a $MUTANT_LOGFILE $LOGFILE
			mv $compilation_results $i.equivalent
		fi
	
	fi	

	echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
    cd $LIBUTIL/src
    mv $filename_orig.orig $filename_orig

	end_time=$(($(date +%s%N)/1000000))
	elapsed="$(($end_time-$start_time))"
	
	echo "elapsed time $elapsed [ms]"

#	if [ $count -eq 2000 ];then
#		break
#	else
#		count=$((count+1))
#	fi
done
