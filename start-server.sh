#!/bin/bash

cd $(dirname $0)
cd lib
exec ruby2.0 server_controller.rb start
