#!/bin/bash
KERNELHEADERS=/lib/modules/$(uname -r)/build/
make -C ${KERNELHEADERS} SUBDIRS=$PWD $1
