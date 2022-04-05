#!/bin/sh

g++ -c os7_read.cpp
g++ -o os7_read os7_read.o -lpthread -lrt
./os7_read
