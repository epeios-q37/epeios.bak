# coding: utf-8
""" 
MIT License

Copyright (c) 2019 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""
 
import workshop._._ as _

VRAI = True
FAUX = False

class Core(_.Core):
  i18n = {
    "Restart": "Recommencer"
  }

P_VISAGE = _.P_FACE
P_TETE = _.P_HEAD
P_TRONC = _.P_TRUNK
P_BRAS_GAUCHE = _.P_LEFT_ARM
P_BRAS_DROIT = _.P_RIGHT_ARM
P_PIED_GAUCHE = _.P_LEFT_LEG
P_PIED_DROIT = _.P_RIGHT_LEG

DICTIONNAIRE = [
  "armoire",
  "boucle",
  "buisson",
  "bureau",
  "chaise",
  "carton",
  "couteau",
  "fichier",
  "garage",
  "glace",
  "journal",
  "kiwi",
  "lampe",
  "liste",
  "montagne",
  "remise",
  "sandale",
  "taxi",
  "vampire",
  "volant",
]

PARTIES_CORPS = (
  P_TETE,
  P_TRONC,
  P_BRAS_GAUCHE,
  P_BRAS_DROIT,
  P_PIED_GAUCHE,
  P_PIED_DROIT
)

FUNCTION_LABELS = {
    _.F_IS_LETTER_IN_WORD: "lettreEstDansMot",
    _.F_GET_MASK: "obtenirMasque",
    _.F_UPDATE_BODY: "majCorps"
}


class Core(_.Core):
  i18n = {
      "Restart": "Recommencer"
  }
  dictionnary = DICTIONNAIRE
  bodyParts = PARTIES_CORPS


_.setAppTitle("Atelier jeu du pendu")



def redessine():
  _.redraw()


def dessinePartieCorps(partie):
  _.drawBodyPart(partie)


def efface():
  _.clear()


def affiche(text):
  _.display(text)


def effaceEtAffiche(text):
  _.clearAndDisplay(text)


def alerte(text):
  _.alert(text)


def demande(text):
  return _.confirm(text)
