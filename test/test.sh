#!/bin/bash
cd ../build
cmake ..
make
cd -
cp ../build/YAN .
rm a.s a.out
./YAN printInt.c
echo Test start ...
./a.out
echo Test success