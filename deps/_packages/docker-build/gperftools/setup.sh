#!/bin/bash

set -e

cp -f -r /usr/local/include/gperftools/* /tmp/home/include/gperftools/
cp -f -r /usr/local/lib/*.a /tmp/home/lib_linux/
cp -f /usr/local/bin/pprof /tmp/home/
