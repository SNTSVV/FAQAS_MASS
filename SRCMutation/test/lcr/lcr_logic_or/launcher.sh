#/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

DIR=$( cd $( dirname $0 ) && pwd )
SRCIROR=/opt/srcirorfaqas
export SRCIROR_LLVM_BIN=$SRCIROR/llvm-build/Release+Asserts/bin
export SRCIROR_LLVM_INCLUDES=$SRCIROR/llvm-build/Release+Asserts/lib/clang/3.8.0/include
export SRCIROR_SRC_MUTATOR=$SRCIROR/SRCMutation/build/mutator

MUTATOR=$SRCIROR/PythonWrappers/mutationClang
FILE=$DIR/test.c

function join_by { local IFS="$1"; shift; echo "$*"; }

total_lines=`cat $FILE | wc -l`

declare -a coverage=()
for it in `seq 1 1 $total_lines`;do coverage+=($it); done

echo "$FILE:"`join_by , ${coverage[@]}` > ~/.srciror/coverage
rm *.mut.*  

source ./lcr_logic_or.sh $MUTATOR $FILE
