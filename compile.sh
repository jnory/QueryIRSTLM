#!/bin/sh

IRSTLM=../../opt/irstlm

g++ -L $IRSTLM/lib -I $IRSTLM/include -O3 -o query-lm query-lm.cpp -lirstlm -lz

