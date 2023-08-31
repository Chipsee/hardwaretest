#!/bin/sh

head=`git rev-parse --verify --short HEAD 2>/dev/null`
LAST_GIT_VERSION="+git$head"

git checkout version.h
sed -i "s/LAST_GIT_HASH/$LAST_GIT_VERSION/g" version.h
