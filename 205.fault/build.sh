#!/bin/bash
KERNELHEADERS=/lib/modules/$(uname -r)/build/
make -C ${KERNELHEADERS} SUBDIRS=$PWD $1

if [ "$1" == "clean" ]
then 
  make clean
else
  make rig
fi
