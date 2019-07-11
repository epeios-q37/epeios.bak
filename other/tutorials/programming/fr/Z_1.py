# coding: utf-8

# From '4.3 Résolution inéquation ax+b=c.py'

import sys
sys.path.append(".")
from workshop.fr.z_1 import *


def resoudreEquationPremierDegre(a,b,c):
    solution = (c-b)/a
    return str(solution)


def resoudreInequationPremierDegre(a,b,ineg,c):
  sol = resoudreEquationPremierDegre(a,b,c)

  if ineg == "≥" :
      if a > 0 :
          return "[" + sol + " ; " + "+∞["
      else:
          return "]-∞" + " ; " + sol + "]"
  if ineg == ">" :
      if a > 0 :
          return "]" + sol +" ; " + "+∞["
      else:
          return "]-∞" +" ; " + sol + "["
  if ineg == "≤" :
      if a > 0 :
          return "]-∞" + " ; " + sol + "]"
      else:
          return "[" + sol + " ; " + "+∞["
  if ineg == "<" :
      if a > 0 :
          return "]-∞" + " ; " + sol + "["
      else:
          return "]" + sol + " ; " + "+∞["


def maFonction(a,b,c,operator):
    affiche("Solution de")
    affiche("{} × x + {} {} {} :".format(a, b, operator, c))
    if operator == '=':
        affiche("x = " + resoudreEquationPremierDegre(a,b,c))
    else:
        affiche("x = " + resoudreInequationPremierDegre(a,b,operator,c))

go(maFonction,True)
"""
Second paramètre :
- si à 'True', il y a un message d'erreur qui est affiché si les valeurs saisies sont incorrectes ;
- si à 'False', ce sont les exceptions Python qui sont affichées.
"""
