# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.c import *

def myFunction(name):
  erase()
  display("Hello, " + name + "!")
  display("Have a good day, " + name + "!")


go(globals())
