#!/bin/bash

constraintFile="constraint.json"
resultFile="result.json"
seed=123
count=1000
num=0
for dir1 in ./testCase/*; do
	for dir2 in $dir1/*; do
		resultFile=$(printf ./SamplingResult/result%02d.json $num)
		# echo $resultFile
		# echo $dir2
		./Project -i $dir2/constraint.json -o $resultFile -s $seed -c $count
		num=$(($num+1))
	done
done
