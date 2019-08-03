# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.z_2a import *


T = 0

down()

for y in range(0, 24):
    setColorHSL(T, 100, 50)
    T += 15
    for x in range(0, 12):
        forward(30)
        turnRight(30)
    turnRight(15)

go()
