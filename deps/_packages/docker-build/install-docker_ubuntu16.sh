#!/bin/bash

set -e

if [ ! $1 ]; then
    echo "usage: ./install-docker_ubuntu16.sh your-account"
    exit 1
fi

apt-get -y install apt-transport-https ca-certificates curl
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
apt-get update
apt-get -y install docker-ce


gpasswd -a $1 docker
reboot
