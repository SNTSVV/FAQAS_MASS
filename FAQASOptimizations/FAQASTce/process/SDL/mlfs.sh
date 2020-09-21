RESULTS=$HOME/MLFS/TCE/runs_sdl

WORKING=$FAQAS/srcirorfaqas/FAQASOptimizations/FAQASTce/process/SDL
CALC=$WORKING/tce_calculator_wo_location.py
EQ_UNIVOCAL=$WORKING/equivalent_univocal.py

for it in {0..5};do
	echo configuration $it
	HASHES=$RESULTS/run_$it/test_runs/hashes.csv
	ORIGINAL=$RESULTS/run_$it/test_runs/original_hash
	python3.6 $CALC $HASHES $ORIGINAL
	mv equivalents equivalents_$it
	mv redundants redundants_$it
done

python3.6 $EQ_UNIVOCAL 5

mv all equivalents_* redundants_* univocal_* MLFS/
