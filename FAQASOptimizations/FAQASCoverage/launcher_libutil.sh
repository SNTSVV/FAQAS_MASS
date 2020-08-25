#!/bin/bash

PROJ=/home/gsl/libutil
TST=$PROJ/tst
PROCESS=/opt/srcirorfaqas/FAQASOptimizations/FAQASCoverage/process_coverage.sh
TYPE=1 # 1 global
DIRS_OUT="can_sniffer|util_app|out"

source $PROCESS $PROJ $TST $TYPE $DIRS_OUT
