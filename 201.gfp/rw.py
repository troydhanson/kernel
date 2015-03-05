#!/usr/bin/python
import os,struct

dev = os.open("dev",os.O_RDWR)
os.write(dev, "hello")
back = os.read(dev,5)
print back
