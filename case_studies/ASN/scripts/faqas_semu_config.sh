
FAQAS_SEMU_OUTPUT_TOPDIR=../OUTPUT

FAQAS_SEMU_GENERATED_MUTANTS_TOPDIR=../OUTPUT/mutants_generation

FAQAS_SEMU_REPO_ROOTDIR=../DOWNLOADED/casestudy

FAQAS_SEMU_ORIGINAL_SOURCE_FILE=../DOWNLOADED/casestudy/test.c

FAQAS_SEMU_COMPILE_COMMAND_SPECIFIED_SOURCE_FILE=./test.c

FAQAS_SEMU_GENERATED_MUTANTS_DIR=$FAQAS_SEMU_GENERATED_MUTANTS_TOPDIR/test

FAQAS_SEMU_BUILD_LLVM_BC()
{
    local in_file=$1
    local out_bc=$2
    # compile
    clang -c -emit-llvm -g -Wall -Werror -Wextra -Wuninitialized -Wcast-qual -Wshadow -Wundef -fdiagnostics-show-option -D_DEBUG -I $FAQAS_SEMU_REPO_ROOTDIR -O0 $in_file -o $out_bc
    return $?
}

FAQAS_SEMU_BUILD_NATIVE()
{
    local in_file=$1
    local out_file=$2
    # compile
    gcc -g -Wall -Werror -Wextra -Wuninitialized -Wcast-qual -Wshadow -Wundef -fdiagnostics-show-option -D_DEBUG -I $FAQAS_SEMU_REPO_ROOTDIR -O0 $in_file -o $out_file
    return $?
}

FAQAS_SEMU_META_MU_TOPDIR=../OUTPUT/meta_mu_topdir

FAQAS_SEMU_GENERATED_META_MU_SRC_FILE=$FAQAS_SEMU_GENERATED_MUTANTS_TOPDIR/test.MetaMu.c

FAQAS_SEMU_GENERATED_META_MU_BC_FILE=$FAQAS_SEMU_GENERATED_MUTANTS_TOPDIR/test.MetaMu.bc

FAQAS_SEMU_GENERATED_TESTS_TOPDIR=../OUTPUT/test_generation

FAQAS_SEMU_TEST_GEN_TIMEOUT=600

