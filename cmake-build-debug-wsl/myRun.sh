#!/bin/bash

constraintFile="constraint.json"
resultFile="result.json"
seed=123
count=1000
num=0
for dir1 in ./testCase/*; do
	for dir2 in $dir1/*; do
		resultFile="./SamplingResult/${dir1##*/}/result_${dir2##*/}.json"
		./Project -i $dir2/constraint.json -o $resultFile -s $seed -c $count
	done
done
