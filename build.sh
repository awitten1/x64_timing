#!/bin/bash

set -eux

iters=10000

nasm -f elf64 -g latency.nasm -o latency.o -Diters=$iters
g++ main.cpp -g -O2 latency.o -DITERS=$iters

