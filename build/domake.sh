#!/bin/bash

./premake5-linux.sh
rm -rf ./obj
make config=debug_x64
make config=release_x64

