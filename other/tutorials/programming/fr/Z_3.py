# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.z_3 import *

import math

def maFonction(polygones,segments):
    polygones = int(polygones)
    segments = int(segments)
    taille = 140 * math.sin(math.pi/segments)
    T = 0
    pasPolygon = 360/polygones
    pasSegment = 360/segments
    for y in range (0, polygones):
        TSL(T, 100, 50)
        T += pasPolygon
        for x in range(0, segments):
            avance(taille)
            tourneDroite(pasSegment)
        tourneGauche(pasPolygon)

go(maFonction)
