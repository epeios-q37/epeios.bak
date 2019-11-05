#coding: utf-8

import workshop.common as _

class _Core(_._.Core):
  i18n = {
    _.label.MissingName: "Veuillez saisir un prénom !"
  }
  body_i18n = {
    _.label.NameToDisplay: "Prénom"
  }

  def __init__(self, dom):
    _.Core.__init__(self, dom)


efface = _.clear
affiche = _.display

def go(globals):
  _.main(lambda dom: _Core(dom), globals,  {_.F_HELLO: "afficheSalutations"}, "Atelier de programmation")
