#!/bin/sh

g++ -c os7_write.cpp
g++ -o os7_write os7_write.o -lpthread -lrt
./os7_write
