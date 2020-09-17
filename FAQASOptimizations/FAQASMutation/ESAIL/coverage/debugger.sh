#!/bin/bash

COVERAGE=/opt/srcirorfaqas/FAQASOptimizations/FAQASMutation/ESAIL/coverage

/opt/rcc-1.3-rc8-gcc/bin/sparc-gaisler-rtems5-gdb -x $COVERAGE/coverage.gdb
