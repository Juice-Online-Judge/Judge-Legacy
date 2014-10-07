#!/bin/bash

cd $(dirname $0)
scons
mv build/executor bin
mv build/libexecutor.so lib/executor
