
    #! /bin/bash
    
    # Use this script to execute the unittests.
    # This accepts one or two arguments:
    # - test file: must be specified
    # - path to rootdir: optional, in case the root dir is different than generation phase.
    # 
    # Example: ./runtest.sh test000001.ktest.c

    set -u
    
    topdir=$(dirname $(readlink -f $0))
    repo_dir=/home/FAQAS/workspace/WORKSPACE/DOWNLOADED
    relative_src=/src.c
    gather_output=false
    [ "${FAQAS_SEMU_GATHER_OUTPUT:-OFF}" = 'on' ] && gather_output=true
    [ $# -eq 1 -o $# -eq 2 ] || { echo 'invlid number of args'; exit 2; }
    test_file=$(readlink -f $1)
    [ $# -eq 2 ] && repo_dir=$(readlink -f $2)
    test -f $test_file || { echo 'test file missing'; exit 2; }
    test -d $repo_dir || { echo 'repo rootdir missing'; exit 2; }
    orig_src=$repo_dir/$relative_src
    test -f $orig_src || { echo "original src missing ($orig_src)"; exit 2; }
    expected_out=$test_file.expected
    got_out=$test_file.got

    # Execute the test
    tmp_file=$test_file.tmp
    cp $orig_src $tmp_file || { echo 'failed to copy orig src'; exit 2; }
    cat $test_file >> $tmp_file || { echo 'failed to append test file content'; exit 2; }
    
    
FAQAS_SEMU_BUILD_CODE_FUNC()
{
    local in_file=$1
    local out_file=$2
    local repo_root_dir=$3
    local compiler=$4
    local flags="$5"
    # compile
    $compiler $flags -g -Wall -Werror -Wextra -Wuninitialized -Wcast-qual -Wshadow -Wundef -fdiagnostics-show-option -D_DEBUG -I . -O0 -I$repo_root_dir -o $out_file $in_file $flags
    return $?
}


    FAQAS_SEMU_BUILD_CODE_FUNC $tmp_file $tmp_file.exe $repo_dir gcc '-x c -lm' || { echo 'build failed for code'; exit 2; }
    $tmp_file.exe > $got_out 2>&1
    rm -f $tmp_file $tmp_file.exe
    if [ $gather_output = true ]; then
        mv $got_out $expected_out
    else
        diff $got_out $expected_out
        exit $?
    fi
    
