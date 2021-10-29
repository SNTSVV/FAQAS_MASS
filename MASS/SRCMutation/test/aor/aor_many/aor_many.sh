#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

MUTATOR=$1
FILE=$2

$MUTATOR --compilation "$FILE -o test" --operators AOR

EXPECTED_PL_PL="double function() {\nint a = 4, b = 5;\nreturn a + b + 3;\n}"
EXPECTED_MN_MN="double function() {\nint a = 4, b = 5;\nreturn a - b - 3;\n}"
EXPECTED_ML_MN="double function() {\nint a = 4, b = 5;\nreturn a * b - 3;\n}"
EXPECTED_PL_ML="double function() {\nint a = 4, b = 5;\nreturn a + b * 3;\n}"
EXPECTED_DV_MN="double function() {\nint a = 4, b = 5;\nreturn a / b - 3;\n}"
EXPECTED_PL_DV="double function() {\nint a = 4, b = 5;\nreturn a + b / 3;\n}"
EXPECTED_MD_MN="double function() {\nint a = 4, b = 5;\nreturn a % b - 3;\n}"
EXPECTED_PL_MD="double function() {\nint a = 4, b = 5;\nreturn a + b % 3;\n}"

tst_pl_pl=`diff test.mut.3.1_7_14.AOR.function.c <(echo -e $EXPECTED_PL_PL) | wc -l`
tst_mn_mn=`diff test.mut.3.2_3_10.AOR.function.c <(echo -e $EXPECTED_MN_MN) | wc -l`
tst_ml_mn=`diff test.mut.3.3_2_10.AOR.function.c <(echo -e "$EXPECTED_ML_MN") | wc -l`
tst_pl_ml=`diff test.mut.3.3_6_14.AOR.function.c <(echo -e "$EXPECTED_PL_ML") | wc -l`
tst_dv_mn=`diff test.mut.3.4_4_10.AOR.function.c <(echo -e $EXPECTED_DV_MN) | wc -l`
tst_pl_dv=`diff test.mut.3.4_8_14.AOR.function.c <(echo -e $EXPECTED_PL_DV) | wc -l`
tst_md_mn=`diff test.mut.3.5_1_10.AOR.function.c <(echo -e $EXPECTED_MD_MN) | wc -l`
tst_pl_md=`diff test.mut.3.5_5_14.AOR.function.c <(echo -e $EXPECTED_PL_MD) | wc -l`

echo $tst_pl_pl
echo $tst_mn_mn
echo $tst_ml_mn
echo $tst_pl_ml
echo $tst_dv_mn
echo $tst_pl_dv
echo $tst_md_mn
echo $tst_pl_md

if [ $tst_pl_pl -eq 0 ] && [ $tst_mn_mn -eq 0 ] && [ $tst_ml_mn -eq 0 ] && [ $tst_pl_ml -eq 0 ] && [ $tst_dv_mn -eq 0 ] && [ $tst_pl_dv -eq 0 ] && [ $tst_md_mn -eq 0 ] && [ $tst_pl_md -eq 0 ];then
    echo -e "${GREEN}TEST aor_many PASSED$NC"
else
    echo -e "${RED}TEST aor_many FAILED$NC"
fi
