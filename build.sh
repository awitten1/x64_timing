#!/bin/bash

set -eux

nasm -f elf64 -g add.nasm -o add.o -Diters=10000
g++ main.cpp -g -O2 add.o