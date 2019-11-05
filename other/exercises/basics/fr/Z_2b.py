# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.z_2b import *


import math

def dessine(polygones, segments):
    polygones = int(polygones)
    segments = int(segments)
    taille = 140 * math.sin(math.pi/segments)
    T = 0
    pasPolygone = 360/polygones
    pasSegment = 360/segments
    baisse()
    for y in range(0, polygones):
        fixeCouleurTSL(T, 100, 50)
        T += pasPolygone
        for x in range(0, segments):
            avance(taille)
            tourneDroite(pasSegment)
        tourneGauche(pasPolygone)


go(globals())
