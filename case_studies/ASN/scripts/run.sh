#! /bin/bash

set -u

TOPDIR=$(dirname $(readlink -f $0))
filename=$(basename $0)
cd_docker_script=$TOPDIR/../../../cd_semu_docker.sh

error_exit()
{
    echo "[$filename] error: $1"
    exit 1
}

faqas_semu_config_file=$TOPDIR/faqas_semu_config.sh
source $faqas_semu_config_file

cd $TOPDIR

output_topdir=$(readlink -f $FAQAS_SEMU_OUTPUT_TOPDIR)
mutants_dir=$(readlink -f $FAQAS_SEMU_GENERATED_MUTANTS_DIR)
meta_mutant_dir=$(readlink -f $FAQAS_SEMU_META_MU_TOPDIR)
meta_mutant_src_file=$(readlink -f $FAQAS_SEMU_GENERATED_META_MU_SRC_FILE)
meta_mutant_bc_file=$(readlink -f $FAQAS_SEMU_GENERATED_META_MU_BC_FILE)
meta_mutant_make_sym_src_file=$(readlink -f $FAQAS_SEMU_GENERATED_META_MU_MAKE_SYM_SRC_FILE)
meta_mutant_make_sym_bc_file=$(readlink -f $FAQAS_SEMU_GENERATED_META_MU_MAKE_SYM_BC_FILE)
gen_test_dir=$(readlink -f $FAQAS_SEMU_GENERATED_TESTS_TOPDIR)
build_bc_func=FAQAS_SEMU_BUILD_LLVM_BC
original_src_file=$(readlink -f $FAQAS_SEMU_ORIGINAL_SOURCE_FILE)
compile_command_spec_src=$FAQAS_SEMU_COMPILE_COMMAND_SPECIFIED_SOURCE_FILE
gen_timeout=$FAQAS_SEMU_TEST_GEN_TIMEOUT

phase=1
if [ $# -eq 1 ]; then
    case "$1" in
        "mutation"|"mutantgeneration")
            phase=1
            ;;
        "compile"|"remove-uncompilable")
            phase=2
            ;;
        "pre-semu"|"presemu"|"pre_semu")
            phase=3
            ;;
        "testgeneration"|"semu")
            phase=4
            ;;
        *)
            error_exit "invalid starting phase argument: $1"
    esac
elif [ $# -ne 0 ]; then
    error_exit "Expecting 0 or 1 argument (starting phase)"
fi

# Create output dir
test -d $output_topdir || mkdir $output_topdir || error_exit "failed to create output dir '$output_topdir'"
test -f $original_src_file || error_exit "Original source file not found: $original_src_file"

############################ FUNCTIONS 

remove_uncompilable_mutants()
{
    # remove the mutants that cannot build
    echo "## Checking mutants compilability ..."
    local failed_compile=0
    local total=$(find $mutants_dir -maxdepth 1 -type f -name *.mut.*.c | wc -l)
    local chunk=$(($total/20))
    local count=0
    for f_path in `find $mutants_dir -maxdepth 1 -type f -name *.mut.*.c`
    do
        if ! $build_bc_func $f_path $f_path.tmp > /dev/null 2>&1
        then
            rm -f $f_path $f_path.tmp
            failed_compile=$(($failed_compile + 1))
        else
            rm -f $f_path.tmp
        fi
        # progress
        count=$(($count+1))
        [ $(($count % $chunk)) -eq 0 ] && echo -n "$count/$total ($failed_compile failed) "
    done

    echo 
    echo "## Failed to compile $failed_compile mutants!"
}

has_semu()
{
    if klee-semu --version 2>&1 | grep -i llvm > /dev/null; then
        return 0
    fi
    return 1
}

run_in_docker()
{
    local cmd_arg=$1
    echo "[$filename] Switching to docker..."
    (set -o pipefail && $cd_docker_script $ws_dir_here "$in_docker_cmd $cmd_arg" 2>&1 | sed "s|$ws_in_docker|$ws_dir_here|g") \
                || error_exit "Failure in docker. Debug in docker with command: $cd_docker_script $ws_dir_here ''"
    echo "To enter docker, run:   $cd_docker_script $ws_dir_here"
    exit
}

############################ EXECUTION

ws_dir_here="$(readlink -f $TOPDIR/..)"
ws_in_docker="/home/FAQAS/workspace"

make_sym_to_append="$TOPDIR/../util_codes/test_gen_wrapping_main.c"

if [ $phase -le 1 ]; then
    echo "[$filename] Calling mutant generation ..."
    # coverage
    alllines=$(seq -s',' 1 $(cat $original_src_file | wc -l))
    mkdir -p $output_topdir/.srciror
    echo "$compile_command_spec_src:$alllines" > $output_topdir/.srciror/coverage
    test -d $mutants_dir && rm -rf $mutants_dir
    HOME=$output_topdir $TOPDIR/create_mutants.sh || error_exit "Mutants creation failed"
    rm -rf $output_topdir/.srciror
    echo "# Finished mutant generation!"
fi

in_docker_cmd="/home/FAQAS/workspace/scripts/$(basename $0)"
if test -f $cd_docker_script; then
    tool_dir=$TOPDIR/../../../
else
    tool_dir=/home/FAQAS/faqas_semu/
fi

if [ $phase -le 2 ]; then
    if has_semu; then
        # remove uncompilable mutants
        remove_uncompilable_mutants
        echo "# Finished Remove uncompilable mutants Done!"
    else
        run_in_docker "compile"
    fi
fi

if [ $phase -le 3 ]; then
    if has_semu; then
        echo "[$filename] Calling pre-semu meta-mutant creation ..."
        # generate meta-mu
        $tool_dir/pre_semu/main.py $meta_mutant_src_file $original_src_file $mutants_dir || error_exit "Pre-semu failed"
        cp $meta_mutant_src_file $meta_mutant_make_sym_src_file || error_exit "copy meta-mu to make_sym failed"
        cat $make_sym_to_append >> $meta_mutant_make_sym_src_file || error_exit "appending to meta-mu to make_sym failed"
        # build meta-mu
        $build_bc_func $meta_mutant_make_sym_src_file $meta_mutant_make_sym_bc_file || error_exit "Building bc file failed ($meta_mutant_make_sym_src_file)"
    else
        run_in_docker "pre-semu"
    fi
fi

if [ $phase -le 4 ]; then
    if has_semu; then
        echo "[$filename] Calling semu test generation ..."
        # call test generation
        test -d $gen_test_dir || mkdir $gen_test_dir || error_exit "Failed to create gen_test_dir $gen_test_dir"
        (set -o pipefail && $tool_dir/underlying_test_generation/main.py $meta_mutant_make_sym_bc_file \
                                                                        --output_top_directory $gen_test_dir \
                                                                        --clear_existing \
                                                                        --generation_timeout $gen_timeout \
                                                                        2>&1 | tee $gen_test_dir/test_gen.log) || error_exit "Test generation failed"
    else
        run_in_docker "testgeneration"
    fi
fi

echo "[$filename] All Done!"
