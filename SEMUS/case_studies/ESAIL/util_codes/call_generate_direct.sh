#! /bin/bash

set -u

error_exit()
{
    echo "error: $1"
    exit 1
}

# specify source under test
src=$1

topdir=$(dirname $(readlink -f $0))

repodir=$topdir/../WORKSPACE/DOWNLOADED/Obsw
generate_script=$topdir/../../ASN/util_codes/generate_direct.py

cd $topdir || error_exit "failed to cd into topdir $topdir"

src_template_folder_suffix="$(echo ${src%.c} | tr '/' '.')"

generate_template_config=$topdir/template_configs/direct-$src_template_folder_suffix.json

$generate_script $repodir/$src direct-$src_template_folder_suffix " -fdata-sections -ffunction-sections -Wall -DASW_VERSION_MAJOR=4 -DASW_VERSION_MINOR=1 -DSVN_REVISION=0 -g -fprofile-arcs -ftest-coverage -DDEBUG -DUNIT_TEST -O0 -g -DTARGET -mcpu=v8 -I$repodir/Source/_Ext/check_for_target/include/ -DLOG_WCET -DSCHEDULER_TRACKING -I$repodir/Source/./. -I$repodir/Source/ApplicationLayer/. -I$repodir/Source/ServiceLayer/. -I$repodir/Source/HighLevelDriverLayer/. -I$repodir/Source/ProtocolLayer/. -I$repodir/Source/LowLevelDriverLayer/. -I$repodir/Source/Utilities/. -I/opt/rtems-4.8-SAT-AIS/sparc-rtems4.8/leon3/lib/include -I$repodir/Source/_Ext/mlfs/include" -c $generate_template_config || error_exit "generation failed for source file $src"

# patch
for f in `ls direct-$src_template_folder_suffix`; do
    sed -i'' '/#include "asn1crt/d' direct-$src_template_folder_suffix/$f
done

live_mutants=$repodir/../live-$src_template_folder_suffix

for f in `ls direct-$src_template_folder_suffix`; do
    to_be_kept=0
    for fut in `awk -F'.' '{print $6}' $live_mutants | sort | uniq`;do
        if [[ $f == "${fut}.wrapping_main.c" ]];then
            to_be_kept=1
        fi        
    done
    if [ $to_be_kept -eq 0 ];then
        rm direct-$src_template_folder_suffix/$f
    fi
done

# patch
for fut in `awk -F'.' '{print $6}' $live_mutants | sort | uniq`;do
    test -f $topdir/template_configs/patch-$src_template_folder_suffix-$fut || continue
    patch direct-$src_template_folder_suffix/$fut.wrapping_main.c $topdir/template_configs/patch-$src_template_folder_suffix-$fut 
done
