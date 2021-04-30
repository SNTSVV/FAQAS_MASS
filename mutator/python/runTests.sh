#/bin/bash

rm results.csv

for setup in "" "export _FAQAS_SINGLETON_FM=TRUE";

do $setup;

echo "_FAQAS_SINGLETON_FM=" $_FAQAS_SINGLETON_FM;

bash cleanTests.sh

pushd tests

bash runTest1.sh
bash runTest2.sh
bash runTest3.sh
bash runTest4.sh
bash runTest5.sh
bash runTest6.sh
bash runTest7.sh
bash runTest8.sh
bash runTest9.sh
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
bash runTest32.sh
bash runTest33.sh
bash runTest34.sh
bash runTest35.sh
bash runTest36.sh
bash runTest37.sh
bash runTest38.sh
bash runTest39.sh
bash runTest40.sh
#bash runTest41.sh
bash runTest42.sh
bash runTest43.sh
# bash runTest44.sh
bash runTest45.sh
bash runTest46.sh
bash runTest47.sh

popd

find . -name "*.results.out" -exec cat > results.csv {} +

echo ""
echo ""
echo "*************************************************************************"
echo "THE FOLLOWING DATA CAN BE FOUND IN THE FILE results_$setup.csv"
echo "*************************************************************************"
echo ""

sort results.csv
mv results.csv results_$setup.csv

done



echo ""
echo ""
echo "*************************************************************************"
echo "THE FOLLOWING DATA CAN BE FOUND IN THE FILE results_$setup.csv"
echo "*************************************************************************"
echo ""

#find . -name "*.results.out" -exec cat > results.csv {} +

#sort results.csv


# bash getCoverage.sh
