# coding: utf-8

import workshop._.c as workshop
import workshop.de._ as _

class _Core:
    i18n = {
      "NameIsMandatory": "Sie müssen einen Name eingeben !"
    }
    bodyI18n = {
      "NameToDisplay": "Anzeigename"
    }


def anzeige(text):
  workshop.display(text)


def go(function):
  workshop.main(function, _Core, _.defaultTitle )