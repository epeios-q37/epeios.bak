# coding: utf-8

import workshop._.z_2 as workshop
import workshop.fr._ as _

def avance(distance):
  workshop.forward(distance)

def tourneDroite(angle):
  workshop.right(angle)

def tourneGauche(angle):
  workshop.left(angle)

def go():
  workshop.main(lambda: None, _.defaultTitle)