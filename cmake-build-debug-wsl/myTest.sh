#!/bin/bash

folder_path="./SamplingResult"

num=0
for dir1 in ./testCase/*; do
	for dir2 in $dir1/*; do
		resultFile="./SamplingResult/${dir1##*/}/result_${dir2##*/}.json"
		python3 evalcns -p $dir2/constraint.json -a $resultFile
		num=$((num+1))
	done
done
