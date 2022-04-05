#!/bin/sh

g++ -c os8_read.cpp
g++ -o os8_read os8_read.o -lpthread -lrt
./os8_read
