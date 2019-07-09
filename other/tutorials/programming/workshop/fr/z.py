# coding: utf-8

import workshop._.z as workshop
import workshop.fr._ as _

class _data:
    bodyI18n = {
      "Solve": "Résoudre",
      "Solution": "Solution"
    }
    i18n = {
      "BadValue": "Seuls des nombres compris entre 100 et -100 exclus sont autorisés !",
      "ACannotBe0": "'a' ne peut être nul !"
    }


def affiche(text):
  workshop.display(text)


def z(function):
  workshop.main(function, _data, _.defaultTitle )