RESULTS=$FAQAS/ESAIL/TCE/runs_sdl

WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/SDL
CALC=$WORKING/tce_calculator.py
EQ_UNIVOCAL=$WORKING/equivalent_univocal.py

for it in {0..4};do
	echo configuration $it
	HASHES=$RESULTS/run_$it/test_runs/hashes.csv
	ORIGINAL=$RESULTS/run_$it/test_runs/original_hash
	python3.6 $CALC $HASHES $ORIGINAL
	mv equivalents equivalents_$it
	mv redundants redundants_$it
done

python3.6 $EQ_UNIVOCAL 4
mv univocal_* equivalents_* redundants_* all ESAIL/
