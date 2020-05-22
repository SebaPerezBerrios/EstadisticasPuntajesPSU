#!/bin/bash

mpirun --hostfile maquinas.txt ./dist/programa puntajes.csv
