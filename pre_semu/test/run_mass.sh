#! /bin/bash

set -u

mass_topdir=/home/ubuntu/workspace/srcirorfaqas/
topdir=$(dirname $(readlink -f $0))

mut_dir=$topdir/mutants

selops=""
if [ $# -eq 1 ]; then
    selops="--operators $1"
else
    [ $# -eq 0 ] || { echo "max 1 arg, mut ops to apply"; exit 1; }
fi

test -d $mut_dir && rm -rf $mut_dir
mkdir $mut_dir

cd $mut_dir

mkdir -p $topdir/.srciror
#echo "$topdir/x.c:26,27" > $topdir/.srciror/coverage
alllines=$(seq -s',' 1 $(cat $topdir/x.c | wc -l))
echo "$topdir/x.c:$alllines" > $topdir/.srciror/coverage

HOME=$topdir python $mass_topdir/PythonWrappers/mutationClang --compilation "gcc $topdir/x.c -o $topdir/x" $selops

mv $topdir/x.mut.* $mut_dir
mv $topdir/meta-mu.info $mut_dir
