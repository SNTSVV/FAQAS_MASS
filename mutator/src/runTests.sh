#/bin/bash

rm -f all_results.csv
rm -f results_NORMAL.csv
rm -f results_SINGLETON.csv

for c in "NORMAL" "SINGLETON";
# for c in "NORMAL";
do

  if [ $c = "SINGLETON" ]; then
  export _FAQAS_SINGLETON_FM="TRUE"
  export FAQAS_COVERAGE_FILE="./faqas_coverage.txt"
  fi

  bash cleanTests.sh

  echo ""

  echo "************<<<<<<||||*||||>>>>>>>************"

  echo "************<<<<<< $c >>>>>>>************"

  echo "************<<<<<<||||*||||>>>>>>>************"

  echo ""

  rm -f results_$c.csv

  pushd tests

  if [ $c = "SINGLETON" ]; then
  bash -x runTest_compilation.sh
  fi

  bash runTest01.sh
  bash runTest02.sh
  bash runTest03.sh
  bash runTest04.sh
  bash runTest05.sh
  bash runTest06.sh
  bash runTest07.sh
  bash runTest08.sh
  bash runTest09.sh
  bash runTest10.sh
  bash runTest11.sh
  bash runTest12.sh
  bash runTest13.sh
  bash runTest14.sh
  bash runTest15.sh
  bash runTest16.sh
  bash runTest17.sh
  bash runTest18.sh
  bash runTest19.sh
  bash runTest20.sh
  bash runTest21.sh
  bash runTest22.sh
  bash runTest23.sh
  bash runTest24.sh
  bash runTest25.sh
  bash runTest26.sh
  bash runTest27.sh
  bash runTest28.sh
  bash runTest29.sh
  bash runTest30.sh
  bash runTest31.sh
  bash runTest32.sh
  bash runTest35.sh
  bash runTest38.sh
  bash runTest39.sh
  bash runTest40.sh
  bash runTest42.sh
  bash runTest44.sh
  bash runTest45.sh
  bash runTest46.sh
  bash runTest47.sh
  bash runTest48.sh
  bash runTest49.sh
  bash runTest50.sh
  bash runTest51.sh
  bash runTest52.sh
  bash runTest53.sh
  bash runTest54.sh
  bash runTest55.sh
  bash runTest56.sh
  bash runTest57.sh
  bash runTest58.sh
  bash runTest60.sh
  bash runTest61.sh
  bash runTest62.sh
  bash runTest63.sh
  bash runTest64.sh
  bash runTest65.sh
  bash runTest66.sh
  bash runTest67.sh
  bash runTest68.sh
  bash runTest69.sh

  popd



  find . -name "*.results.out" -exec cat > results_$c.csv {} +


done

echo ""
echo ""
echo "*************************************************************************"
echo "THE FOLLOWING DATA CAN BE FOUND IN THE FILE all_results.csv"
echo "*************************************************************************"
echo ""

cat results_*.csv > all_results.csv

sort all_results.csv
