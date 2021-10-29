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

repodir=$topdir/../WORKSPACE/DOWNLOADED/libutil
generate_script=$topdir/../../ASN/util_codes/generate_direct.py

cd $topdir || error_exit "failed to cd into topdir $topdir"

src_template_folder_suffix="$(echo ${src%.c} | tr '/' '.')"

generate_template_config=$topdir/template_configs/direct-$src_template_folder_suffix.json

$generate_script $repodir/$src direct-$src_template_folder_suffix " -std=gnu99 -m64 -O1 -Wall -Wextra -Wshadow -Wcast-align -Wwrite-strings -Wno-unused-parameter -fPIC -I$repodir/src -I$repodir/build/src -I$repodir/include -I$repodir/build/include -I$repodir/build/include/gs -I$repodir/include/gs -I$repodir/include/deprecated -I$repodir/build/include/deprecated -I$repodir/build/include/deprecated/gs/gosh -I$repodir/include/deprecated/gs/gosh" -c $generate_template_config || error_exit "generation failed for source file $src"

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
