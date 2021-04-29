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
mutants_dir=$FAQAS_SEMU_GENERATED_MUTANTS_DIR
meta_mutant_dir=$FAQAS_SEMU_META_MU_TOPDIR
meta_mutant_src_file=$FAQAS_SEMU_GENERATED_META_MU_SRC_FILE
meta_mutant_bc_file=$FAQAS_SEMU_GENERATED_META_MU_BC_FILE
gen_test_dir=$FAQAS_SEMU_GENERATED_TESTS_TOPDIR
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
        "pre-semu"|"presemu"|"pre_semu")
            phase=2
            ;;
        "testgeneration"|"semu")
            phase=3
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

if [ $phase -le 1 ]; then
    echo "[$filename] Calling mutant generation ..."
    # coverage
    alllines=$(seq -s',' 1 $(cat $original_src_file | wc -l))
    mkdir -p $output_topdir/.srciror
    echo "$compile_command_spec_src:$alllines" > $output_topdir/.srciror/coverage
    HOME=$output_topdir $TOPDIR/create_mutants.sh || error_exit "Mutants creation failed"
    rm -rf $output_topdir/.srciror
fi

has_semu()
{
    if klee-semu --version 2>&1 | grep -i llvm > /dev/null; then
        return 0
    fi
    return 1
}
in_docker_cmd="/home/FAQAS/workspace/scripts/$(basename $0)"
if test -f $cd_docker_script; then
    tool_dir=$TOPDIR/../../../
else
    tool_dir=/home/FAQAS/faqas_semu/
fi

if [ $phase -le 2 ]; then
    if has_semu; then
        echo "[$filename] Calling pre-semu meta-mutant creation ..."
        # generate meta-mu
        $tool_dir/pre_semu/main.py $meta_mutant_src_file $original_src_file $mutants_dir || error_exit "Pre-semu failed"
        # build meta-mu
        $build_bc_func $meta_mutant_src_file $meta_mutant_bc_file || error_exit "Building bc file failed"
    else
        echo "[$filename] Switching to docker..."
        $cd_docker_script $TOPDIR/.. "$in_docker_cmd pre-semu" || error_exit "Failure in docker"
    fi
fi

if [ $phase -le 3 ]; then
    if has_semu; then
        echo "[$filename] Calling semu test generation ..."
        # call test generation
        $tool_dir/underlying_test_generation/main.py $meta_mutant_bc_file --output_top_directory $gen_test_dir --clear_existing --generation_timeout $gen_timeout || error_exit "Test generation failed"
    else
        echo "[$filename] Switching to docker..."
        $cd_docker_script $TOPDIR/.. "$in_docker_cmd testgeneration" || error_exit "Failure in docker"
    fi
fi

echo "[$filename] All Done!"
