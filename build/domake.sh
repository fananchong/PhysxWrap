#!/bin/bash

./premake5-linux.sh
rm -rf ./obj
make config=release_x64

