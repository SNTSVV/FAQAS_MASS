#! /bin/bash

TOPDIR=$(dirname $(readlink -f $0))

data_dir=$TOPDIR/data

script_data_location=$TOPDIR/../MLFS/scripts
generate_template_script=$TOPDIR/../ASN/util_codes/generate_direct.sh

workdir_top=$TOPDIR
workspace_dir=$workdir_top/"WORKSPACE"
downloaded_dir=$workspace_dir/"DOWNLOADED"
output_dir=$workspace_dir/"OUTPUT"
scripts_dir=$workdir_top/"scripts"
util_codes_dir=$workdir_top/"util_codes"

expected_dir=$TOPDIR/"expected"
expected_templates=$expected_dir/"templates"
expected_testgen_out=$expected_dir/"OUTPUT"

workdir_list="$workspace_dir $scripts_dir $util_codes_dir"

system_failure() {
    echo "@SYSTEM-FAILURE: $1"
    exit 2
}

test_failure() {
    printf "@test-failure: $1\n"
    exit 1
}

cleanup() {
    for wdir in $workdir_list; 
    do
        if [ -d $wdir ]; then
            rm -rf $wdir || sudo rm -rf $dir || system_failure "failed to remove workdir $wdir"
        fi
    done
}

setup() {
    # create workspace dir
    cleanup
    for wdir in $workdir_list; 
    do
        mkdir $wdir || system_failure "failed to create workdir $wdir"
    done
    # setup the scenario
    ## create dirs
    mkdir $workspace_dir || system_failure "Failed to create WORKSPACE dir"
    mkdir $downloaded_dir || system_failure "Failed to create DOWNLOADED dir"
    mkdir $scripts_dir || system_failure "failed to create scripts dir"
    mkdir $util_codes_dir || system_failure "failed to create util_codes dir"
    ## copy files
    cp $data_dir/*.c $downloaded_dir || system_failure "failed to copy source files"
    cp $data_dir/compile_commands.json $scripts_dir || system_failure "failed to copy compile_commands file"
    cp $data_dir/faqas_semu_config.sh $scripts_dir || system_failure "failed to copy config file"
    cp $script_data_location/run.sh $scripts || system_failure "failed to copy the run.sh script"
    cp $script_data_location/docker_run.sh $scripts || system_failure "failed to copy the docker_run.sh script"
    sed -i'' 's|MLFS|tests|g' $scripts/docker_run.sh || system_failure "failed to sed docker_run.sh file"
    cp $script_data_location/create_mutants.sh $scripts || system_failure "failed to copy the create_mutants.sh script"
}

get_env_args() {
    if [ "${USE_FAQAS_SEMU_DOCKER_IMAGE:-}" = "ON" ]; then
        use_faqas_semu_docker=true
    else
        use_faqas_semu_docker=false
    fi
}

gen_templates() {
    local repodir=$downloaded_dir
    cd $repodir || system_failure "faile to cd into repodir $repodir"
    local cfiles=$(find -type f -name '*.c' | sed 's|^./||g')
    cd - > /dev/null

    for src in $cfiles; do
        src_template_folder_suffix="$(echo ${src%.c} | tr '/' '.')"
        $generate_template_script $repodir/$src $util_codes_dir/direct-$src_template_folder_suffix " -I$repodir" -c $data_dir/generate_template_config.json \
                || test_failure "template generation failed for source file $src\n#1 TEMPLATE GENERATION test failed :( #" 
        # patch
        for f in `ls direct-$src_template_folder_suffix`; do
            sed -i'' '/#include "asn1crt/d' direct-$src_template_folder_suffix/$f
        done

        diff $expected_templates/direct-$src_template_folder_suffix $util_codes_dir/direct-$src_template_folder_suffix \
            || test_failure "generated template mismatch for $src\n#1 TEMPLATE GENERATION test failed :( #"
    done

}

call_generation_script() {
    local src=$1
    if [ $use_faqas_semu_docker = true ]; then
        ENV_FAQAS_SEMU_SRC_FILE=$src $scripts/docker_run.sh || test_failure "test generation pipeline crash\n#2 TEST GENERATION PIPELINE test failed :( #"
    else
        ENV_FAQAS_SEMU_SRC_FILE=$src $scripts/run.sh || test_failure "test generation pipeline crash\n#2 TEST GENERATION PIPELINE test failed :( #"
    fi
}

check_results() {
    local src=$1
    diff -r $expected_testgen_out $output_dir/$src || test_failure "test generation pipeline output files difference\n#2 TEST GENERATION PIPELINE test failed :( #"
}

test_generation_pipeline() {
    local repodir=$downloaded_dir
    cd $repodir || system_failure "faile to cd into repodir $repodir"
    local cfiles=$(find -type f -name '*.c' | sed 's|^./||g')
    cd - > /dev/null

    for src in $cfiles; do
        call_generation_script $src
        check_results $src
    done
}

##############################

use_faqas_semu_docker=false
get_env_args

echo "#* Test SETUP #"
setup

echo "#1 TESTING TEMPLATE GENERATION ... #"
gen_templates
echo "#1 TEMPLATE GENERATION test passed :) #"

echo "#2 TESTING TEST GENERATIOn PIPELINE ... #"
test_generation_pipeline
echo "#2 TEST GENERATION PIPELINE test passed :) #"

echo "#* Test CLEANUP #"
cleanup
