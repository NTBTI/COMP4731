#!/bin/bash
while [ true ]; do
    n=$RANDOM
    let "n = 1 + n % 5"
    echo "${n} --> ${1}"
    sleep $n
done