#! /bin/bash

set -u

error_exit()
{
    echo "error: $1"
    exit 1
}

topdir=$(dirname $(readlink -f $0))

repodir=$topdir/../WORKSPACE/DOWNLOADED/MLFS-QDP_I1_R1/BL-SC/E1356-GTD-BL-01_I1_R2
generate_script=$topdir/../../ASN/util_codes/generate_direct.py

cd $repodir || error_exit "faile to cd into repodir $repodir"
cfiles=$(find -type f -name '*.c' | sed 's|^./||g')
cd $topdir

for src in $cfiles; do
    src_template_folder_suffix="$(echo ${src%.c} | tr '/' '.')"
    $generate_script $repodir/$src direct-$src_template_folder_suffix " -I$repodir/include -I$repodir/libm/common -I$repodir/libm/math -I$repodir/libm/mlfs" \
            || error_exit "generation failed for source file $src" 
done

