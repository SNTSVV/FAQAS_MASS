#! /bin/bash
# run the test

set -u

error_exit(){
    echo "ERROR: $1"
    exit 1
}

topdir=$(dirname $(readlink -f $0))

test_subject=$topdir/../main.py
test -f $test_subject || error_exit "test_subject missing ($test_subject)"

tmpout=$topdir/tmpoutput.tmp
test -d $tmpout && rm -rf $tmpout
mkdir $tmpout

export PATH=$topdir/mock_exe:$PATH

yes | python $test_subject $topdir/bcmock.bc --output_top_directory $tmpout --clear_existing || error_exit "call to test subject failed"

