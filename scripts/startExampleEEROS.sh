#!/bin/bash


# coredump
ulimit -c unlimited
rm -f ./core


sudo ../buildx86/exampleEeros -f ../eni/10_ENI_2Elmo_1000ms_extended.xml -i8254x 1 1 -perf -v 0 -dcmmode mastershift -b 400

