#!/bin/sh

me=$(dirname $0)
#${CPP-cpp} -nostdinc $* | $me/python.bin
cat $* | $me/python.bin
