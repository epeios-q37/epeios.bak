# coding: utf-8

# All files under MIT license
# Copyright (c) 2019 Claude SIMON (https://q37.info/s/rmnmqd49)
# See  'LICENSE' file.

import sys,os

# Default language: English.
# Replace below "en" by "fr" to switch to French.
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

success = False

print("\nYou can change the language by modifying the 'main.py' file.")

while not success:
    exercise = input(_messages["input"][lang].format(" (a,b…,l)"))

    exerciseFilename = lang + "." + exercise.lower()

    if True:  # Simplifies debugging when set to False
        try:
            __import__(exerciseFilename)
        except ImportError:
            print("'" + exerciseFilename + ".py'" +
                    _messages["error"][lang])
        else:
            success = True
    else:
        __import__(exerciseFilename)
