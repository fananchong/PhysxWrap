#!/bin/bash

set -e

cp -f /boost_1_64_0/stage/lib/*.a /tmp/home/lib_linux
cp -f -r /boost_1_64_0/boost /tmp/home/include/
