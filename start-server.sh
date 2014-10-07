#!/bin/bash

cd $(dirname $0)
cd lib
exec ruby server.rb
