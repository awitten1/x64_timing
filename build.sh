#!/bin/bash

set -eux

nasm -f elf64 -g latency.nasm -o latency.o -Diters=1000
g++ main.cpp -g -O2 latency.o

