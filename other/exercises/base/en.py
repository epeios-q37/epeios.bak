#coding: utf-8

from workshop.en import *

def displayGreetings(name):
  erase()
  display("Hello, " + name + "!")
  display("Have a good day, " + name + "!")

go(globals()) 
