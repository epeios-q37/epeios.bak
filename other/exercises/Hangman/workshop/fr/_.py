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

USER_ITEM_LABELS = {
  _.UC_HANGMAN: "Pendu",
  _.UV_SHOW_SECRET_WORD: "MONTRER_MOT_SECRET",
  _.UF_GET_MASK: "donnerMasque",
  _.UF_HANDLE_GUESS: "traiterPioche",
  _.UF_IS_LETTER_IN_WORD: "lettreEstDansMot",
  _.UF_RESET: "reinitialiser",
  _.UF_UPDATE_BODY: "majCorps",
  _.UF_PICK_WORD: "choisirMot",
  _.UF_ACONNECT: "AConnexion",
  _.UF_ASUBMIT: "APioche",
  _.UF_ARESTART: "ARedemarrage"}


_.setI18n({
    _.label.Restart: "Recommencer",
    _.label.SecretWord: "Mot secret",
    _.label.Letter: "Pioche ",
    _.label.Expected: "Attendu ",
    _.label.Obtained: "Obtenu ",
    # '_.label.True': syntax error!
    _.label.__dict__["True"]: "vrai",
    # '_.label.False': syntax error!
    _.label.__dict__["False"]: "faux",
})

_.setDictionary(DICTIONNAIRE)
_.setBodyParts(PARTIES_CORPS)


_.setAppTitle("Atelier du jeu du pendu")


def redessiner():
  _.redraw()


def dessinerPartieCorps(partie):
  _.drawBodyPart(partie)


def effacer():
  _.clear()


def afficher(text):
  _.display(text)


def effacerEtAfficher(text):
  _.clearAndDisplay(text)


def notifier(text):
  _.alert(text)


def demander(text):
  return _.confirm(text)
