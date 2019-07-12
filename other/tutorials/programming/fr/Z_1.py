# coding: utf-8

# From '4.3 Résolution inéquation ax+b=c.py'

from workshop.fr.z_1 import *
import sys
sys.path.append(".")


traiteErreurs = True
"""
- si à 'True', vérification de la validité des valeurs en amont,
- si à 'False', pas de vérifications; si valeur invalide, ce sont les exceptions Python qui sont affichées.
"""


def resoudreEquationPremierDegre(a, b, c):
    solution = (c-b)/a
    return str(solution)


def resoudreInequationPremierDegre(a, b, ineg, c):
    sol = resoudreEquationPremierDegre(a, b, c)

    if ineg == "≥":
        if a > 0:
            return "[" + sol + " ; " + "+∞["
        else:
            return "]-∞" + " ; " + sol + "]"
    if ineg == ">":
        if a > 0:
            return "]" + sol + " ; " + "+∞["
        else:
            return "]-∞" + " ; " + sol + "["
    if ineg == "≤":
        if a > 0:
            return "]-∞" + " ; " + sol + "]"
        else:
            return "[" + sol + " ; " + "+∞["
    if ineg == "<":
        if a > 0:
            return "]-∞" + " ; " + sol + "["
        else:
            return "]" + sol + " ; " + "+∞["


def maFonction(a, b, c, operator):
    # N'est utile que lorsque 'traiteErreur' est à 'False'.
    if (a == 0):
        alerte("'a' doit être différent de 0 !")
        return
    # Il y a encore d'autres tests à réaliser.

    efface()
    affiche("Solution de")
    affiche("{} × x + {} {} {} :".format(a, b, operator, c))
    if operator == '=':
        affiche("x = " + resoudreEquationPremierDegre(a, b, c))
    else:
        affiche("x = " + resoudreInequationPremierDegre(a, b, operator, c))


go(maFonction, traiteErreurs)
