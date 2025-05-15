#!/bin/bash

set -e

DOCKER_TMP_NAME=cbox-busybox
CONTAINERS_FOLDER=./runtime
TMP_FOLDER=/tmp
TMP_BUSYBOX_PATH=$TMP_FOLDER/busybox.tar.xz

NAME=$1


if [ -z "$NAME" ]; then
    echo "usage: $0 <CONTAINER NAME>"
    exit 1
fi

CONTAINER_FOLDER=$CONTAINERS_FOLDER/$NAME

if [ -e $CONTAINER_FOLDER ]; then
    echo "container '$NAME' already exists"
    exit 1
fi

set -x

docker pull busybox:latest
docker stop $DOCKER_TMP_NAME || true && docker rm $DOCKER_TMP_NAME || true
docker create --name $DOCKER_TMP_NAME busybox:latest
docker export $DOCKER_TMP_NAME -o $TMP_BUSYBOX_PATH
docker remove $DOCKER_TMP_NAME

mkdir -p $CONTAINER_FOLDER
tar -xf $TMP_BUSYBOX_PATH -C $CONTAINER_FOLDER
rm $TMP_BUSYBOX_PATH
