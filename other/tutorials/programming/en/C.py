# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.c import *

def myFunction(name):
  display("Hello, " + name + "!")
  display("Have a good day, " + name + "!")


go(myFunction)
