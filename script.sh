#!/bin/bash

#gcc -o HPC_project DF_Opt.c -lm
#./HPC_project

for i in $(seq 0 2 299)
do
	sed "s/XYX/$i/g" grafico.gp > grafico-$i-map.gp
	gnuplot grafico-$i-map.gp
done
