#!/bin/bash

for i in $(seq 1 12);
do
    echo $i
    for j in $(seq 1 10);
    do
        ./rk3_8_par.x $i
    done
done
