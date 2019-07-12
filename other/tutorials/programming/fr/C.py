# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.c import *
from workshop.fr.dispalcon import *

def maFonction(nom):
    efface()
    affiche("Bonjour, " + nom + " !")
    affiche("Bonne journée, " + nom + " !")

go(maFonction)
