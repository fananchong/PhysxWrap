#!/bin/bash

set -e

if [ ! $1 ]; then
    echo "usage: ./install-docker_centos7.sh your-account"
    exit 1
fi

yum install -y yum-utils
yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
yum makecache fast
yum -y install docker-ce
systemctl start docker.service
systemctl enable docker.service

gpasswd -a $1 docker
systemctl restart docker.service
pkill X
