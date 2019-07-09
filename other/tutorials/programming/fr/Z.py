# coding: utf-8

# From '4.3 Résolution inéquation ax+b=c.py'

from workshop.fr.z import *


def resoudreEquationPremierDegre(a,b,c):
    solution = (c-b)/a
    return str(solution)


def resoudreInequationPremierDegre(a,b,ineg,c):
  sol = resoudreEquationPremierDegre(a,b,c)

  if ineg == ">=" :
      if a > 0 :
          return "[" + sol + " ; " + "+∞["
      else:
          return "]-∞" + " ; " + sol + "]"
  if ineg == ">" :
      if a > 0 :
          return "]" + sol +" ; " + "+∞["
      else:
          return "]-∞" +" ; " + sol + "["
  if ineg == "<=" :
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
    affiche("La solution est :")
    if operator == '=':
        affiche("x = " + resoudreEquationPremierDegre(a,b,c))
    else:
        affiche("x = " + resoudreInequationPremierDegre(a,b,operator,c))

go(maFonction,True)
"""
Second paramètre :
- si à 'True', il y a un message d'erreur qui est affiché si les velurs sisies sont incorrectes ;
- si à 'False', c'est les exceptions Python qui sont affichées.
"""
