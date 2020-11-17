#!/bin/bash

FOLDERS=RQ5

find $FOLDERS -name '*.csv' -exec sed -i '1s/^/rate;iteration;total;killed;timeout;live;ms\n/' {} \;
