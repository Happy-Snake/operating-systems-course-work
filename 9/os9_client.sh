#!/bin/sh

g++ -c os9_client.cpp
g++ -o os9_client os9_client.o -lpthread -lrt
./os9_client
