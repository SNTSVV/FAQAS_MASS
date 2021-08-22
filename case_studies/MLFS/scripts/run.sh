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

cd $TOPDIR

[ "${ENV_FAQAS_SEMU_SRC_FILE:-}" != "" ] || error_exit "You must specify the source file using the env var 'ENV_FAQAS_SEMU_SRC_FILE'"
src_template_folder_suffix="$(echo ${ENV_FAQAS_SEMU_SRC_FILE%.c} | tr '/' '.')"
echo "# Running fore source file $ENV_FAQAS_SEMU_SRC_FILE..."

faqas_semu_config_file=$TOPDIR/faqas_semu_config.sh
source $faqas_semu_config_file

test -f $FAQAS_SEMU_REPO_ROOTDIR/$ENV_FAQAS_SEMU_SRC_FILE || error_exit "The specified source does not exist ($FAQAS_SEMU_REPO_ROOTDIR/$ENV_FAQAS_SEMU_SRC_FILE)"

if test -f $TOPDIR/$FAQAS_SEMU_CASE_STUDY_WORKSPACE/faqas_semu_config.sh; then
    faqas_semu_config_file=$TOPDIR/$FAQAS_SEMU_CASE_STUDY_WORKSPACE/faqas_semu_config.sh
    echo "[run.sh] using overriding faqas_semu_config_file $faqas_semu_config_file."
    source $faqas_semu_config_file
else
    echo "[run.sh] using default faqas_semu_config_file $faqas_semu_config_file."
fi

output_topdir=$(readlink -fm $FAQAS_SEMU_OUTPUT_TOPDIR)
mutants_dir=$(readlink -fm $FAQAS_SEMU_GENERATED_MUTANTS_DIR)
meta_mutant_dir=$(readlink -fm $FAQAS_SEMU_META_MU_TOPDIR)
meta_mutant_src_file=$(readlink -fm $FAQAS_SEMU_GENERATED_META_MU_SRC_FILE)
meta_mutant_bc_file=$(readlink -fm $FAQAS_SEMU_GENERATED_META_MU_BC_FILE)
meta_mutant_make_sym_top_dir=$(readlink -fm $FAQAS_SEMU_GENERATED_META_MU_MAKE_SYM_TOP_DIR)
gen_test_dir=$(readlink -fm $FAQAS_SEMU_GENERATED_TESTS_TOPDIR)
build_bc_func=FAQAS_SEMU_BUILD_LLVM_BC
original_src_file=$(readlink -fm $FAQAS_SEMU_ORIGINAL_SOURCE_FILE)
compile_command_spec_src=$FAQAS_SEMU_COMPILE_COMMAND_SPECIFIED_SOURCE_FILE
gen_timeout=$FAQAS_SEMU_TEST_GEN_TIMEOUT
semu_heuristics_config=$FAQAS_SEMU_HEURISTICS_CONFIG
gen_memory_limit=$FAQAS_SEMU_TEST_GEN_MAX_MEMORY
gen_stop_on_mem_limit=$FAQAS_SEMU_STOP_TG_ON_MEMORY_LIMIT
gen_max_memory_inhibit=$FAQAS_SEMU_TG_MAX_MEMORY_INHIBIT

help="
Run as following:

./run.sh [<starting-phase>] [<mutants-list-file> <output-dir-for-pre-semu-and-semu>]

Where:
    - starting-phase: is the phase from which to starting
    - mutants-list-file: is the file containing the list of mutants to use during the phases pre-semu and semu. This must be a realtive path to the case study dir 
                                    $(readlink -f $FAQAS_SEMU_CASE_STUDY_WORKSPACE) (must be contained there).
    - output-dir-for-pre-semu-and-semu: directory to store the output of pre-semu and semu phases, when the mutants list is specified.
                                    This directory must be specified relative to the case study dir $(readlink -f $FAQAS_SEMU_CASE_STUDY_WORKSPACE) (must be contained there).
                                    For example specifying 'OUTPUT/my-output' will result in putting the output in '$(readlink -f $FAQAS_SEMU_CASE_STUDY_WORKSPACE/OUTPUT/my-output)'

--------------------------------------------------------------------------------------

Example 1:  scripts/run.sh 
Example 2:  scripts/run.sh compile
Example 3:  scripts/run.sh pre-semu DOWNLOADED/live_mutants OUTPUT/live_mutants_output

"

phase=1
mutants_list_file=""
custom_semu_pre_output=""
custom_meta_mutant_src_file=""
custom_meta_mutant_bc_file=""
custom_meta_mutant_make_sym_top_dir=""
custom_gen_test_dir=""
if [ $# -eq 1 -o $# -eq 3 ]; then
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
        "extract-unittests"|"unittests")
            phase=5
            ;;
        "--help"|"-h")
            echo "$help"
            exit 0
            ;;
        *)
            error_exit "invalid starting phase argument: $1. $help"
    esac
    if [ $# -eq 3 ]; then
        raw_mutants_list_file="$2"
        raw_custom_semu_pre_output="$3"

        cd $FAQAS_SEMU_CASE_STUDY_WORKSPACE || error_exit "cd $FAQAS_SEMU_CASE_STUDY_WORKSPACE failed"
        mutants_list_file=$(readlink -fm $raw_mutants_list_file)
        custom_semu_pre_output=$(readlink -fm $raw_custom_semu_pre_output)
        cd - > /dev/null || error_exit "cd - failed"
        
        custom_meta_mutant_src_file=$custom_semu_pre_output/mutants_generation/$(basename $meta_mutant_src_file)
        custom_meta_mutant_bc_file=$custom_semu_pre_output/mutants_generation/$(basename $meta_mutant_bc_file)
        custom_meta_mutant_make_sym_top_dir=$custom_semu_pre_output/mutants_generation/$(basename $meta_mutant_make_sym_top_dir)
        custom_gen_test_dir=$custom_semu_pre_output/$(basename $gen_test_dir)
        test -f $mutants_list_file || "specified mutant list file missing ($mutants_list_file). $help"
        [ "$custom_semu_pre_output" != "" ] || error_exit "empty custom out dir"
    fi
elif [ $# -ne 0 ]; then
    error_exit "Expecting 0, 1 or3 arguments: $help"
fi

echo "[run.sh] Running with: PHASE=$phase, OPTIONAL_MUTANT_LIST='$mutants_list_file', OPTIONAL_OUTDIR='$custom_semu_pre_output'"

# Create output dir
test -d $output_topdir || mkdir -p $output_topdir || error_exit "failed to create output dir '$output_topdir'"
test -f $original_src_file || error_exit "Original source file not found: $original_src_file"

trap "exit 1" INT

############################ FUNCTIONS 

remove_uncompilable_mutants()
{
    # remove the mutants that cannot build
    echo "## Checking mutants compilability ..."
    local failed_compile=0
    local total=$(find $mutants_dir -maxdepth 1 -type f -name *.mut.*.c | wc -l)
    local chunk=$(($total/20))
    [ $chunk -eq 0 ] && chunk=$total
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
        [ $(($count % $chunk)) -eq 0 ] && stdbuf -o0 echo -n "$count/$total ($failed_compile failed) "
    done

    echo 
    echo "## Failed to compile $failed_compile mutants!"
}

check_function_has_mutants()
{
    local function_name=$1
    # if mutant list, use it else use all mutants
    if [ "$mutants_list_file" != "" ]; then
        cat $mutants_list_file | rev | cut -d'.' -f2 | rev | grep "^$function_name$" > /dev/null && return 0
    else
        find $mutants_dir -maxdepth 1 -type f -name *.mut.*.c | rev | cut -d'.' -f2 | rev | grep "^$function_name$" > /dev/null && return 0
    fi
    return 1
}

apply_pre_semu_template()
{
    local base_meta_mutant_src_file=$1
    local base_meta_mutant_bc_file=$2
    local func_template_path=$3
    local func_meta_dir=$4

    local func_template=$(basename $func_template_path)
    test -d $func_meta_dir || mkdir -p $func_meta_dir || error_exit "Failed to create func_meta_dir $func_meta_dir" 
    local meta_mutant_make_sym_src_file=$func_meta_dir/$(basename $base_meta_mutant_src_file)
    local meta_mutant_make_sym_bc_file=$func_meta_dir/$(basename $base_meta_mutant_bc_file)
            
    cp $base_meta_mutant_src_file $meta_mutant_make_sym_src_file || error_exit "copy meta-mu to make_sym failed"
    cat $func_template_path >> $meta_mutant_make_sym_src_file || error_exit "appending to meta-mu to make_sym failed"
    # build various meta-mu
    $build_bc_func $meta_mutant_make_sym_src_file $meta_mutant_make_sym_bc_file \
                                                                || error_exit "Building bc file failed ($meta_mutant_make_sym_src_file)"
}

invoke_semu()
{
    local meta_mutant_make_sym_bc_file=$1
    local func_gen_test_dir=$2

    local other_args=""
    if [ "$gen_stop_on_mem_limit" = "ON" ]; then
        other_args+=" --stop_on_memory_limit"
    fi
    if [ "$gen_max_memory_inhibit" = "ON" ]; then
        other_args+=" --max_memory_inhibit"
    fi

    # call test generation
    test -d $func_gen_test_dir || mkdir -p $func_gen_test_dir || error_exit "Failed to create func_gen_test_dir $func_gen_test_dir"
    (set -o pipefail && $tool_dir/underlying_test_generation/main.py $meta_mutant_make_sym_bc_file \
                                                                            --output_top_directory $func_gen_test_dir \
                                                                            --clear_existing \
                                                                            --generation_timeout $gen_timeout \
                                                                            --semu_heuristics_config "$semu_heuristics_config" \
                                                                            --max_memory_MB $gen_memory_limit $other_args \
                                                                            2>&1 | tee $func_gen_test_dir/test_gen.log) \
                                                                                                || error_exit "Test generation failed"
}

produce_unittest()
{
    local ktest_indir=$1
    local unittest_outdir=$2
    local func_template_path=$3
    $tool_dir/ktest_to_unittest/main.py $unittest_outdir $ktest_indir $func_template_path || error_exit "Ktest to unittest failed for ktest dir $ktest_indir"
    # generate test execution script (take a test file and path to repo, execute script and print out diff and ret non zero if fails, nothing if pass and return 0)
    local exec_script_str="
    #! /bin/bash
    topdir=\$(dirname \$(readlink -f \$0))
    orig_src=$original_src_file
    gather_output=false
    [ \"\${FAQAS_SEMU_GATHER_OUTPUT:-OFF}\" = 'on' ] && gather_output=true
    [ \$# -eq 1 -o \$# -eq 2 ] || { echo 'invlid number of args'; exit 2; }
    test_file=\$(readlink -f \$1)
    [ \$# -eq 2 ] && orig_src=\$(readlink -f \$2)
    test -f \$test_file || { echo 'test file missing'; exit 2; }
    test -f \$orig_src || { echo 'original src missing'; exit 2; }
    expected_out=\$test_file.expected
    got_out=\$test_file.got

    # Execute the test
    tmp_file=\$test_file.tmp
    cp \$orig_src \$tmp_file || { echo 'failed to copy orig src' }
    cat \$test_file >> \$tmp_file
    
    $FAQAS_SEMU_BUILD_CODE_FUNC_STR

    FAQAS_SEMU_BUILD_CODE_FUNC \$tmp_file \$tmp_file.exe \$repo_dir gcc '' || { echo 'build failed for code'; exit 2; }
    \$tmp_file.exe > \$got_out
    rm -f \$tmp_file.exe
    if [ \$gather_output = true ]; then
        mv \$got_out \$expected_out
    else
        diff \$got_out \$expected_out
        exit $?
    fi
    "
    echo $exec_script_str > $unittest_outdir/runtest.sh
    chmod +x $unittest_outdir/runtest.sh
    # build each test, execute and gather assertion output
    for f in `ls $unittest_outdir/*.c`; do
        FAQAS_SEMU_GATHER_OUTPUT=on $unittest_outdir/runtest.sh $unittest_outdir/$f || error_exit "failed getting expected out for $unittest_outdir/$f"
    done
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
    if [ "$mutants_list_file" != "" ]; then
        local _tmp=
        cmd_arg+=" "$(echo $mutants_list_file | sed "s|^$ws_dir_here|$ws_in_docker|g")
        cmd_arg+=" "$(echo $custom_semu_pre_output | sed "s|^$ws_dir_here|$ws_in_docker|g")
    fi
    echo "[$filename] Switching to docker..."
    (set -o pipefail && $cd_docker_script $ws_dir_here "$in_docker_cmd $cmd_arg" 2>&1 | sed "s|$ws_in_docker|$ws_dir_here|g") \
                || error_exit "Failure in docker. Debug in docker with command: $cd_docker_script $ws_dir_here ''"
    echo "To enter docker, run:   $cd_docker_script $ws_dir_here"
    exit
}

############################ EXECUTION

ws_dir_here="$(readlink -f $TOPDIR/..)"
ws_in_docker="/home/FAQAS/workspace"

make_sym_to_append_top_dir="$TOPDIR/../util_codes/"

if [ $phase -le 1 ]; then
    echo "[$filename] Calling mutant generation ..."
    # coverage
    alllines=$(seq -s',' 1 $(cat $original_src_file | wc -l))
    mkdir -p $output_topdir/.srciror
    echo "$compile_command_spec_src:$alllines" > $output_topdir/.srciror/coverage
    test -d $mutants_dir && rm -rf $mutants_dir
    HOME=$output_topdir $TOPDIR/create_mutants.sh || error_exit "Mutants creation failed (code $?)"
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
        if [ "$mutants_list_file" != "" ]; then
            test -d $(dirname $custom_meta_mutant_src_file) || mkdir -p $(dirname $custom_meta_mutant_src_file) || \
                                                                    error_exit "faile to create dir $(dirname $custom_meta_mutant_src_file)"
                                                                    
            $tool_dir/pre_semu/main.py $custom_meta_mutant_src_file $original_src_file $mutants_dir --target-mutant-list $mutants_list_file || \
                                                                                                            error_exit "Pre-semu failed"
            
            echo "# generate Templates specific Meta Mu files ..."
            category="direct-$src_template_folder_suffix"
            category_dir=$make_sym_to_append_top_dir/$category
            for func_template in `ls $category_dir`
            do
                g_func_name=$(echo $func_template | cut -d'.' -f1)
                check_function_has_mutants $g_func_name || continue
                echo "[$filename] pre-semu processing function $g_func_name ..."
                g_func_template_path=$category_dir/$func_template
                g_func_meta_dir=$custom_meta_mutant_make_sym_top_dir/$category/$g_func_name
                apply_pre_semu_template $custom_meta_mutant_src_file $custom_meta_mutant_bc_file $g_func_template_path $g_func_meta_dir
            done
        else
            $tool_dir/pre_semu/main.py $meta_mutant_src_file $original_src_file $mutants_dir || error_exit "Pre-semu failed"

            echo "# generate Templates specific Meta Mu files ..."
            for category in `ls $make_sym_to_append_top_dir`
            do
                category_dir=$make_sym_to_append_top_dir/$category
                test -d $category_dir || continue
                for func_template in `ls $category_dir`
                do
                    g_func_name=$(echo $func_template | cut -d'.' -f1)
                    echo "[$filename] pre-semu processing function $g_func_name ..."
                    g_func_template_path=$category_dir/$func_template
                    g_func_meta_dir=$meta_mutant_make_sym_top_dir/$category/$g_func_name
                    apply_pre_semu_template $meta_mutant_src_file $meta_mutant_bc_file $g_func_template_path $g_func_meta_dir
                done
            done
        fi
    else
        run_in_docker "pre-semu"
    fi
fi

if [ $phase -le 4 ]; then
    if has_semu; then
        echo "[$filename] Calling semu test generation ..."
        if [ "$mutants_list_file" != "" ]; then
            category="direct-$src_template_folder_suffix"
            category_dir=$custom_meta_mutant_make_sym_top_dir/$category
            for g_func_name in `ls $category_dir`
            do
                check_function_has_mutants $g_func_name || continue
                echo "[$filename] semu processing function $g_func_name ..."
                g_func_meta_dir=$category_dir/$g_func_name
                g_meta_mutant_make_sym_bc_file=$g_func_meta_dir/$(basename $custom_meta_mutant_bc_file)
                g_func_gen_test_dir=$custom_gen_test_dir/$category/$g_func_name
                invoke_semu $g_meta_mutant_make_sym_bc_file $g_func_gen_test_dir
            done
        else
            for category in `ls $meta_mutant_make_sym_top_dir`
            do
                category_dir=$meta_mutant_make_sym_top_dir/$category
                for g_func_name in `ls $category_dir`
                do
                    echo "[$filename] semu processing function $g_func_name ..."
                    g_func_meta_dir=$category_dir/$g_func_name
                    g_meta_mutant_make_sym_bc_file=$g_func_meta_dir/$(basename $meta_mutant_bc_file)
                    g_func_gen_test_dir=$gen_test_dir/$category/$g_func_name
                    invoke_semu $g_meta_mutant_make_sym_bc_file $g_func_gen_test_dir
                done
            done
        fi
    else
        run_in_docker "testgeneration"
    fi
fi

if [ $phase -le 5 ]; then
    if has_semu; then
        echo "[$filename] Calling ktest to unittest conversion ..."
        # produce unittest
        if [ "$mutants_list_file" != "" ]; then
            category="direct-$src_template_folder_suffix"
            category_dir=$make_sym_to_append_top_dir/$category
            for func_template in `ls $category_dir`
            do
                g_func_name=$(echo $func_template | cut -d'.' -f1)
                check_function_has_mutants $g_func_name || continue
                echo "[$filename] produce unittests processing function $g_func_name ..."
                g_func_template_path=$category_dir/$func_template
                g_func_gen_ktest_dir=$custom_gen_test_dir/$category/$g_func_name/FAQAS_SEMu-out/semu/tests_files
                g_func_gen_unittest_dir=$custom_gen_test_dir/$category/$g_func_name/FAQAS_SEMu-out/produced-unittests
                rm -rf $g_func_gen_unittest_dir || error_exit "Failed to remove existing unittest dir output"
                mkdir $g_func_gen_unittest_dir || error_exit "Failed to create unittest dir"
                produce_unittest $g_func_gen_ktest_dir $g_func_gen_unittest_dir $g_func_template_path
            done
        else
            for category in `ls $make_sym_to_append_top_dir`
            do
                category_dir=$make_sym_to_append_top_dir/$category
                test -d $category_dir || continue
                for func_template in `ls $category_dir`
                do
                    g_func_name=$(echo $func_template | cut -d'.' -f1)
                    echo "[$filename] produce unittests processing function $g_func_name ..."
                    g_func_template_path=$category_dir/$func_template
                    g_func_gen_ktest_dir=$gen_test_dir/$category/$g_func_name/FAQAS_SEMu-out/semu/tests_files
                    g_func_gen_unittest_dir=$gen_test_dir/$category/$g_func_name/FAQAS_SEMu-out/produced-unittests
                    rm -rf $g_func_gen_unittest_dir || error_exit "Failed to remove existing unittest dir output"
                    mkdir $g_func_gen_unittest_dir || error_exit "Failed to create unittest dir"
                    produce_unittest $g_func_gen_ktest_dir $g_func_gen_unittest_dir $g_func_template_path
                done
            done
        fi
    else
        run_in_docker "unittests"
    fi
fi

echo "[$filename] All Done!"
