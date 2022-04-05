#!/bin/sh

g++ -c os6_write.cpp
g++ -o os6_write os6_write.o -lpthread -lrt
./os6_write
