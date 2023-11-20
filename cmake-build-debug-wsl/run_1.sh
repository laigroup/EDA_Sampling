#!/bin/bash

while [ "$1" != "" ]; do
  if [ $1 == "-input" ]; then
    shift
    constraintFile=$1
    shift
  elif [ $1 == "-output" ]; then
    shift
    resultFile=$1
    shift
  elif [ $1 == "-seed" ]; then
    shift
    seed=$1
    shift
  elif [ $1 == "-count" ]; then
    shift
    count=$1
    shift
  fi
done

./Project -i $constraintFile -o $resultFile -s $seed -c $count