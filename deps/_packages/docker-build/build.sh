#!/bin/bash

set -ex

out_path=$PWD/../../

docker build -t plengine/build-centos7 ./centos7.3

docker build -t plengine/build-boost ./boost
docker run --rm -v $out_path:/tmp/home plengine/build-boost /bin/bash "/setup.sh"

docker build -t plengine/build-physx ./physx
docker run --rm -v $out_path:/tmp/home plengine/build-physx /bin/bash "/setup.sh"

