# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.z_2 import *

T = 0;

for y in range (0, 24):
    fixeCouleurTSL(T,100,50)
    T += 15
    for x in range(0, 12):
        avance(30)
        tourneDroite(30)
    tourneDroite(15)

go()
