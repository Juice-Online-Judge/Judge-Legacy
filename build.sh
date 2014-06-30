#!/bin/bash

cd $(dirname $0)
cd src/build
cmake ..
make
mv executor ../../bin
mv libexecute.so ../../ext
