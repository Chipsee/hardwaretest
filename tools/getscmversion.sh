#!/bin/sh
head=`git rev-parse --verify --short HEAD 2>/dev/null`
echo $head
