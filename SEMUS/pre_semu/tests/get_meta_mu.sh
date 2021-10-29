#! /bin/bash

set -u

error_exit() {
    echo "ERROR: $1"
    exit 1
}

topdir=$(dirname $(readlink -f $0))

presemu_exec=$(readlink -f $topdir/../main.py)
test -f "$presemu_exec" || error_exit "cannot find main.py"

# remove the mutants that cannot build
echo "# Checking compilability ..."
failed_compile=0
for f in `ls $topdir/mutants`
do
    echo $f | grep "^x\.mut" > /dev/null || continue
    f_path=$topdir/mutants/$f
    if ! gcc $f_path -o $f_path.tmp > /dev/null 2>&1
    then
        rm -f $f_path $f_path.tmp
        failed_compile=$(($failed_compile + 1))
    else
        rm -f $f_path.tmp
    fi
done

echo "# Failed to compile $failed_compile mutants!"

$presemu_exec $topdir/x.MetaMu.c $topdir/x.c $topdir/mutants || error_exit "pre-semu failed"

echo "# Generation Successfull!"

# try to build
clang -c -emit-llvm $topdir/x.MetaMu.c -o $topdir/x.MetaMu.bc || error_exit "metaMu compilation failed"
