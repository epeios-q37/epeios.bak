# coding: utf-8

import sys
import os

lang = "fr"

_messages = {
    "input": {
        "en": "Please enter the exercise ID: ",
        "de": "Bitte geben Sie die Übungs-ID ein: ",
        "fr": "Veuillez saisir l'identifiant de l'exercice : ",
    },
    "error": {
        "en": " not found!",
        "de": " nicht gefunden!",
        "fr": " introuvable !",
    }
}

def go(exercise=None):
    if not exercise:
        exercise = input("\n" + _messages["input"][lang])

#  exerciseFilename = "Exercise" + exercise.upper() + "_" + lang

    exerciseFilename = lang + "." + exercise.upper()

    if True:  # Simplifies debuggins when set to False
        try:
            __import__(exerciseFilename)
        except ImportError:
            exit("\n'" + exerciseFilename + ".py'" +
                 _messages["error"][lang] + "\n")
    else:
        __import__(exerciseFilename)
