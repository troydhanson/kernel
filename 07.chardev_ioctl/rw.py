#!/usr/bin/python
import os,struct

dev = os.open("dev",os.O_RDWR)
os.write(dev, "hello")
count = os.read(dev,8)
c = struct.unpack("l",count)
print c[0]
