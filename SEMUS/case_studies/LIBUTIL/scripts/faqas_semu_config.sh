
FAQAS_SEMU_CASE_STUDY_TOPDIR=../

FAQAS_SEMU_CASE_STUDY_WORKSPACE=$FAQAS_SEMU_CASE_STUDY_TOPDIR/WORKSPACE

FAQAS_SEMU_OUTPUT_TOPDIR=$FAQAS_SEMU_CASE_STUDY_WORKSPACE/OUTPUT/"${ENV_FAQAS_SEMU_SRC_FILE%.c}"

FAQAS_SEMU_GENERATED_MUTANTS_TOPDIR=$FAQAS_SEMU_OUTPUT_TOPDIR/mutants_generation

FAQAS_SEMU_REPO_ROOTDIR=$FAQAS_SEMU_CASE_STUDY_WORKSPACE/DOWNLOADED/libutil

FAQAS_SEMU_ORIGINAL_SOURCE_FILE=$FAQAS_SEMU_REPO_ROOTDIR/"${ENV_FAQAS_SEMU_SRC_FILE}"

FAQAS_SEMU_COMPILE_COMMAND_SPECIFIED_SOURCE_FILE=./"${ENV_FAQAS_SEMU_SRC_FILE}"

FAQAS_SEMU_GENERATED_MUTANTS_DIR=$FAQAS_SEMU_GENERATED_MUTANTS_TOPDIR/"${ENV_FAQAS_SEMU_SRC_FILE%.c}"

FAQAS_SEMU_BUILD_CODE_FUNC_STR='
FAQAS_SEMU_BUILD_CODE_FUNC()
{
    local in_file=$1
    local out_file=$2
    local repo_root_dir=$3
    local compiler=$4
    local flags="$5"
    # compile
    $compiler $flags -g -std=gnu99 -m64 -O0 -Wall -Wextra -Wshadow -Wcast-align -Wwrite-strings -Wno-unused-parameter -I$repo_root_dir/src -I$repo_root_dir/src/gosh -I$repo_root_dir/src/zip -I$repo_root_dir/src/watchdog  -I$repo_root_dir/build/src/watchdog -I$repo_root_dir/build/src -I$repo_root_dir/include -I$repo_root_dir/build/include -I$repo_root_dir/include/gs -I$repo_root_dir/build/include/gs -I$repo_root_dir/include/deprecated -I$repo_root_dir/build/include/deprecated -I$repo_root_dir/include/deprecated/gs/gosh -I$repo_root_dir/build/include/deprecated/gs/gosh -I$repo_root_dir/include -o $out_file $in_file $flags
    return $?
}
'

FAQAS_SEMU_BUILD_LLVM_BC()
{
    local in_file=$1
    local out_bc=$2
    eval "$FAQAS_SEMU_BUILD_CODE_FUNC_STR"
    FAQAS_SEMU_BUILD_CODE_FUNC $in_file $out_bc $FAQAS_SEMU_REPO_ROOTDIR clang '-c -emit-llvm'
    return $?
}

FAQAS_SEMU_META_MU_TOPDIR=$FAQAS_SEMU_OUTPUT_TOPDIR/meta_mu_topdir

FAQAS_SEMU_GENERATED_META_MU_SRC_FILE=$FAQAS_SEMU_GENERATED_MUTANTS_TOPDIR/"${ENV_FAQAS_SEMU_SRC_FILE%.c}".MetaMu.c

FAQAS_SEMU_GENERATED_META_MU_BC_FILE=$FAQAS_SEMU_GENERATED_MUTANTS_TOPDIR/"${ENV_FAQAS_SEMU_SRC_FILE%.c}".MetaMu.bc

FAQAS_SEMU_GENERATED_META_MU_MAKE_SYM_TOP_DIR=$FAQAS_SEMU_GENERATED_MUTANTS_TOPDIR/"MakeSym-TestGen-Input"

FAQAS_SEMU_GENERATED_TESTS_TOPDIR=$FAQAS_SEMU_OUTPUT_TOPDIR/test_generation

# timeout in seconds
FAQAS_SEMU_TEST_GEN_TIMEOUT=300

# This is the config for SEMU heuristics. The accepted values of 'PSS' are 'RND' for random and 'MDO' for minimum distance to output
FAQAS_SEMU_HEURISTICS_CONFIG='{
        "PL": "0",
        "CW": "4294967295",
        "MPD": "0",
        "PP": "1.0",
        "NTPM": "5",
        "PSS": "RND"
}'

# max Test Generation memory in MB
FAQAS_SEMU_TEST_GEN_MAX_MEMORY=2000

# Set to 'ON' to stop test generation when the memory limit is reached
FAQAS_SEMU_STOP_TG_ON_MEMORY_LIMIT='OFF'

# Set this to 'ON' so thae the states the sate fork is disabled when the memory limit is reached, to avoid going much over it
FAQAS_SEMU_TG_MAX_MEMORY_INHIBIT="ON"
