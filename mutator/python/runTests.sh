#/bin/bash
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
bash runTest32.sh
bash runTest33.sh
bash runTest34.sh

popd

bash getCoverage.sh
