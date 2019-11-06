# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.z_2b import *


import math

def draw(polygons, segments):
    down()
    polygons = int(polygons)
    segments = int(segments)
    length = 140 * math.sin(math.pi/segments)
    H = 0
    polygonStep = 360/polygons
    segmentStep = 360/segments
    for y in range(0, polygons):
        setColorHSL(H, 100, 50)
        H += polygonStep
        for x in range(0, segments):
            forward(length)
            turnRight(segmentStep)
        turnLeft(polygonStep)


go(globals())
