# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.c import *


def afficheSalutations(nom):
    efface()
    affiche("Bonjour, " + nom + " !")
    affiche("Bonne journée, " + nom + " !")
    

go(globals())
