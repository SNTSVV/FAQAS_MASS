#!/bin/bash

all=$1
size=200

count=0
iteration=1
while IFS="" read -r p || [ -n "$p" ];do
	if [ $count -lt $size ];then
		echo $p >> m$iteration
		count=$((count+1))
	else
		count=0
		iteration=$((iteration+1))
	fi
done < $all

