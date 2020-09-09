RESULTS=$HOME/PARAM_GSL/TCE/runs

CALC=$HOME/srcirorfaqas/FAQASOptimizations/FAQASTce/process/tce_calculator.py
EQ_UNIVOCAL=$HOME/srcirorfaqas/FAQASOptimizations/FAQASTce/process/equivalent_univocal.py

for it in {0..5};do
	echo configuration $it
	HASHES=$RESULTS/run_$it/test_runs/hashes.csv
	ORIGINAL=$RESULTS/run_$it/test_runs/original_hash
	python3.6 $CALC $HASHES $ORIGINAL
	mv equivalents equivalents_$it
	mv redundants redundants_$it
done

python3.6 $EQ_UNIVOCAL 5

#rm all
#rm equivalents_*
#rm redundants_*
