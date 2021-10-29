#!/bin/bash

for tst in `find . -name 'launcher.sh'`;do
    dir=`echo $tst | sed 's:/launcher.sh::'`
    pushd $dir
    source ./launcher.sh
    popd
done
