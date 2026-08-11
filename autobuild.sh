#!/bin/bash
set -x
rm -rf `pwd`/build/*
mkdir -p build
cd `pwd`/build &&
cmake .. &&
make
