#!/bin/bash

cd $(dirname $0)
scons
mkdir -p "bin"
mkdir -p lib/executor
mv build/executor bin
mv build/libexecutor.so lib/executor
mkdir -p "run/exe" > /dev/null 2>&1
mkdir -p "run/in" > /dev/null 2>&1
mkdir -p "run/out" > /dev/null 2>&1
mkdir -p "run/ans" > /dev/null 2>&1
