# coding: utf-8

from workshop.fr.z_3 import *

def maFonction(polygones,segments,taille):
    T = 0
    pasPolygon = 360/polygones
    pasSegment = 360/segments
    for y in range (0, polygones):
        TSL(T, 100, 50)
        T += pasPolygon
        for x in range(0, segments):
            avance(taille)
            tourneDroite(pasSegment)
        tourneDroite(pasPolygon)

go(maFonction)
