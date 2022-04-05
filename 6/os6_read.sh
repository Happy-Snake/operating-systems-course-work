#!/bin/sh

g++ -c os6_read.cpp
g++ -o os6_read os6_read.o -lpthread -lrt
./os6_read
