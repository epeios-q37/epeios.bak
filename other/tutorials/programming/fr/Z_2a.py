# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.z_2a import *

T = 0

baisse()

for y in range(0, 24):
    fixeCouleurTSL(T, 100, 50)
    T += 15
    for x in range(0, 12):
        avance(30)
        tourneDroite(30)
    tourneDroite(15)


# Autre
"""
for i in range(0, 9):
  tourneDroite(100)
  avance(50)
  tourneGauche(140)
  avance(50)    
"""

go()
