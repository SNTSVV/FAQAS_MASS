#!/bin/bash

PROJ=$1
PROJ_SRC=$2
PROJ_BUILD=$3
PROJ_ORIGINAL_BUILD=$4
FLAG=$5
SRC_MUTANTS=$6
COMPILED=$7
EXEC_DIR=$8
COMMAND=$9

LOGFILE=$EXEC_DIR/main.log
mkdir -p $EXEC_DIR
touch $LOGFILE

shopt -s extglob
trap "exit" INT

count=0

for i in $(find $SRC_MUTANTS -name '*.c');do
    start_time=$(($(date +%s%N)/1000000))

    file_wo_opt=${i//$SRC_MUTANTS/}
    mutant_name="$(basename -- ${file_wo_opt%.*})"
    file_wo_mut_end=.${file_wo_opt%%.*}.c

    filename="$(basename -- ${file_wo_mut_end%.*})"

    function=$(echo $mutant_name | sed 's/.*\.//g')
   
    echo $function
 
    filename_orig=$(echo $file_wo_mut_end | sed -e "s/\(.*\)$filename\//\1/g")

    mutant_path=$EXEC_DIR/$mutant_name
    mkdir -p $mutant_path

    MUTANT_LOGFILE=$mutant_path/logfile

    touch $MUTANT_LOGFILE

    echo "------------------------------------" 2>&1 | tee -a $MUTANT_LOGFILE
    echo "Mutant: "$i 2>&1 | tee -a $MUTANT_LOGFILE

    cd $PROJ_SRC

    # replacing mutant by original source
    echo cp $filename_orig $filename_orig.orig 2>&1 | tee -a $MUTANT_LOGFILE
    cp $filename_orig $filename_orig.orig

    echo cp $i $filename_orig 2>&1 | tee -a $MUTANT_LOGFILE
    cp $i $filename_orig

    cd $PROJ
    echo ASDASDASDASSADDSA $COMMAND[@]
    "${COMMAND[@]}" 2>&1 | tee -a $MUTANT_LOGFILE

    RET_CODE=${PIPESTATUS[0]}                                                                                                          
    
    if [ $RET_CODE -eq 1 ]; then
        echo "Error: mutant could not be compiled" 2>&1 | tee -a $MUTANT_LOGFILE
        echo $mutant_name"      not compiled" 2>&1 | tee -a $LOGFILE
        
        touch $i.notcompiled
    else
        echo "Success: mutant compiled" 2>&1 | tee -a $MUTANT_LOGFILE
        echo $mutant_name"      compiled" 2>&1 | tee -a $LOGFILE
    
        cp $PROJ_BUILD/$COMPILED $i.$COMPILED
        
        equivalence=`diff --binary $PROJ_ORIGINAL_BUILD $i.$COMPILED | wc -l`

        if [ $equivalence -eq 1 ];then
            echo "$mutant_name not equivalent with original" 2>&1 | tee -a $MUTANT_LOGFILE $LOGFILE  
        
            redundant=0
            for m in $(find $SRC_MUTANTS -name "*$filename*$function*.$COMPILED");do
#                echo comparing $m with $filename
                if [[ "$m" == "${i}.${COMPILED}" ]];then
                    continue
                fi

                redundancy=`diff --binary $i.$COMPILED $m | wc -l`
                
                if [ $redundancy -eq 0 ];then
                    echo "$mutant_name redundant with $m" 2>&1 | tee -a $MUTANT_LOGFILE $LOGFILE
                    mv $i.$COMPILED $i.redundant
                    redundant=1
                                                                                                                                       
                    break
                fi
            done
             if [ $redundant -eq 0 ];then
                echo "$mutant_name not redundant" 2>&1 | tee -a $MUTANT_LOGFILE $LOGFILE
            fi          
        else
            echo "$mutant_name equivalent with original" 2>&1 | tee -a $MUTANT_LOGFILE $LOGFILE
            mv $i.$COMPILED $i.equivalent
        fi

    fi

    echo "Replacing original source "$i 2>&1 | tee -a $MUTANT_LOGFILE
    cd $PROJ_SRC
    mv $filename_orig.orig $filename_orig

    end_time=$(($(date +%s%N)/1000000))
    elapsed="$(($end_time-$start_time))"
    
    echo "elapsed time $elapsed [ms]"

   if [ $count -eq 10 ];then
       break
   else
       count=$((count+1))
   fi
done

# reporting
compiled=`find $SRC_MUTANTS -name "*.${COMPILED}" | wc -l`
equivalent=`find $SRC_MUTANTS -name '*.equivalent' | wc -l`
redundant=`find $SRC_MUTANTS -name '*.redundant' | wc -l`
notcompiled=`find $SRC_MUTANTS -name '*.notcompiled' | wc -l`

echo compiled $compiled
echo equivalent $equivalent 
echo redundant $redundant  
echo notcompiled $notcompiled 
echo total $((compiled+equivalent+redundant+notcompiled)) 

final_list=$EXEC_DIR/list$FLAG
touch $final_list

find $SRC_MUTANTS -name "*.$COMPILED" >> $final_list                                                       
find $SRC_MUTANTS -name '*.equivalent' >> $final_list                                                       
find $SRC_MUTANTS -name '*.redundant' >> $final_list                                                       
find $SRC_MUTANTS -name '*.notcompiled' >> $final_list                                                       

echo "backing up compiled mutants" 2>&1 | tee -a $MUTANT_LOGFILE
cd /opt/mutations
GZIP=-9 tar czf compiled.tar.gz src-mutants/
mv compiled.tar.gz $EXEC_DIR

