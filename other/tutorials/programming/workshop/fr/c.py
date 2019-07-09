# coding: utf-8

import workshop._.c as workshop
import workshop.fr._ as _

class _Core:
    i18n = {
      "NameIsMandatory": "Il faut saisir un nom !"
    }
    bodyI18n = {
      "NameToDisplay": "Nom à afficher"
    }


def affiche(text):
  workshop.display(text)


def go(function):
  workshop.main(function, _Core, _.defaultTitle )