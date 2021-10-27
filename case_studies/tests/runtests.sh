#! /bin/bash

TOPDIR=$(dirname $(readlink -f $0))

data_dir=$TOPDIR/data

script_data_location=$TOPDIR/../MLFS/scripts
generate_template_script=$TOPDIR/../ASN/util_codes/generate_direct.py

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
            rm -rf $wdir || sudo rm -rf $wdir || system_failure "failed to remove workdir $wdir"
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
    mkdir $downloaded_dir || system_failure "Failed to create DOWNLOADED dir"

    ## copy files
    cp $data_dir/*.c $downloaded_dir || system_failure "failed to copy source files"
    cp $data_dir/compile_commands.json $scripts_dir || system_failure "failed to copy compile_commands file"
    cp $data_dir/faqas_semu_config.sh $scripts_dir || system_failure "failed to copy config file"
    cp $script_data_location/run.sh $scripts_dir || system_failure "failed to copy the run.sh script"
    cp $script_data_location/docker_run.sh $scripts_dir || system_failure "failed to copy the docker_run.sh script"
    sed -i'' 's|MLFS|tests|g' $scripts_dir/docker_run.sh || system_failure "failed to sed docker_run.sh file"
    cp $script_data_location/create_mutants.sh $scripts_dir || system_failure "failed to copy the create_mutants.sh script"
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
        for f in `ls $util_codes_dir/direct-$src_template_folder_suffix`; do
            sed -i'' '/#include "asn1crt/d' $util_codes_dir/direct-$src_template_folder_suffix/$f
        done

        diff $expected_templates/direct-$src_template_folder_suffix $util_codes_dir/direct-$src_template_folder_suffix \
            || test_failure "generated template mismatch for $src\n#1 TEMPLATE GENERATION test failed :( #"
    done

}

call_generation_script() {
    local src=$1
    if [ $use_faqas_semu_docker = true ]; then
        ENV_FAQAS_SEMU_SRC_FILE=$src $scripts_dir/docker_run.sh || test_failure "test generation pipeline crash\n#2 TEST GENERATION PIPELINE test failed :( #"
    else
        ENV_FAQAS_SEMU_SRC_FILE=$src $scripts_dir/run.sh || test_failure "test generation pipeline crash\n#2 TEST GENERATION PIPELINE test failed :( #"
    fi
}

check_results() {
    local src=$1
    local exp_path=$expected_testgen_out/${src%.c}
    local got_path=$output_dir/${src%.c}
    local exp_mut=$exp_path/mutants_generation
    local got_mut=$got_path/mutants_generation
    local exp_tg=$exp_path/test_generation
    local got_tg=$got_path/test_generation
    # Check mutant related files
    for efp in `find $exp_mut -type f`; do
        local gfp=$(echo $efp | sed "s|^$exp_mut|$got_mut|g")
        diff  $efp $gfp || test_failure "test generation pipeline output files difference between $efp and $gfp\n#2 TEST GENERATION PIPELINE test failed :( #"
    done
    # check test related files
    exp_unit=$(find $exp_tg -type d -name produced-unittests)
    got_unit=$(find $got_tg -type d -name produced-unittests)
    [ $(ls $exp_unit | wc -l) -eq $(ls $got_unit | wc -l) ] || test_failure "mismatch number of files in unittest folder ($exp_unit and $got_unit)\n#2 TEST GENERATION PIPELINE test failed :( #"
    [ $(ls $exp_unit/*.ktest.c | wc -l) -eq $(ls $got_unit/*.ktest.c | wc -l) ] || test_failure "mismatch number of unittest files ($exp_unit and $got_unit)\n#2 TEST GENERATION PIPELINE test failed :( #"
    for e_utf in `ls $exp_unit/*.ktest.c`
    do
        found=0
        for g_utf in `ls $got_unit/*.ktest.c`
        do
            if diff $exp_unit/$e_utf $got_unit/$g_utf > /dev/null
            then
                found=1
                break
            fi 
        done
        [ $found -eq 0 ] && test_failure "No matching obtained test for expected test $exp_unit/$e_utf\n#2 TEST GENERATION PIPELINE test failed :( #"
    done 
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
echo

echo "#1 TESTING TEMPLATE GENERATION ... #"
gen_templates
echo "#1 TEMPLATE GENERATION test passed :) #"
echo

echo "#2 TESTING TEST GENERATIOn PIPELINE ... #"
test_generation_pipeline
echo "#2 TEST GENERATION PIPELINE test passed :) #"
echo

echo "ALL Tests passed"
echo
echo "#* Test CLEANUP #"
cleanup
