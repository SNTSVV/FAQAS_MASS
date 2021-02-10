RESULTS=$HOME/tce_test_runs

CALC=/opt/srcirorfaqas/FAQASOptimizations/FAQASTce/process/tce_calculator.py

for it in {0..5};do
	echo configuration $it
	HASHES=$RESULTS/run_$it/test_runs/hashes.csv
	ORIGINAL=$RESULTS/run_$it/test_runs/original_hash
	python3.5 $CALC $HASHES $ORIGINAL
	mv equivalents equivalents_$it
	mv redundants redundants_$it
	mv all all_$it
done

#rm all
#rm equivalents_*
#rm redundants_*
