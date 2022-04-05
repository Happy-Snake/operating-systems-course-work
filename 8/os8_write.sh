#!/bin/sh

g++ -c os8_write.cpp
g++ -o os8_write os8_write.o -lpthread -lrt
./os8_write
