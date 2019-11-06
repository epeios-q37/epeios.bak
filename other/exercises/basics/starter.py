# coding: utf-8

import sys
import os

lang = "en"

_messages = {
    "input": {
        "en": "Please enter the exercise ID{}: ",
        "de": "Bitte geben Sie die Übungs-ID ein{}: ",
        "fr": "Veuillez saisir l'identifiant de l'exercice{} : ",
    },
    "error": {
        "en": " not found!",
        "de": " nicht gefunden!",
        "fr": " introuvable !",
    }
}
    
try:
    input = raw_input
except NameError:
    pass

def go(exercise=None):
    if not exercise:
        exercise = input("\n" + _messages["input"][lang].format(" (a,b,c,z_1,z_2a,z_2b,z_3)"))

#  exerciseFilename = "Exercise" + exercise.upper() + "_" + lang

    exerciseFilename = lang + "." + exercise[:1].upper() + exercise[1:].lower()

    if True:  # Simplifies debuggins when set to False
        try:
            __import__(exerciseFilename)
        except ImportError:
            exit("\n'" + exerciseFilename + ".py'" +
                 _messages["error"][lang] + "\n")
    else:
        __import__(exerciseFilename)
