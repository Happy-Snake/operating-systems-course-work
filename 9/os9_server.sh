#!/bin/sh

g++ -c os9_server.cpp
g++ -o os9_server os9_server.o -lpthread -lrt
./os9_server
