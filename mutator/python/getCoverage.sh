#!/bin/bash

rm CompleteCoverageReport.csv

tests=`ls tests`
for x in $tests
do
echo "getting data from $x"

if [ -f "tests/${x}/hlreport.csv" ]; then
    echo "********* hlreport.csv exists! *********"

    cp tests/${x}/hlreport.csv caniolino_${x}.csv

    echo "********* hlreport.csv retrieved *********"

fi

x=$((x+1))
done

cat caniolino_*.csv > totalbody.csv

cat coverageReportHeader.csv totalbody.csv > CompleteCoverageReport.csv

rm caniolino_*.csv

rm totalbody.csv
