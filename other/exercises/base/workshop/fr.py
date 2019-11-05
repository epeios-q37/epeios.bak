import workshop.common as _

class _Core(_._.Core):
    i18n = {
        _.L_NAME_IS_MANDATORY: "Il faut saisir un nom !"
    }
    bodyI18n = {
        _.L_NAME_TO_DISPLAY: "Ton prénom"
    }

    def __init__(self, dom):
        _.Core.__init__(self, dom)


def efface():
  _.clear()

def affiche(text):
  _.display(text)

def go(globals):
    _.main(lambda dom: _Core(dom), globals,  {_.F_HELLO: "afficheSalutations",}, "Atelier de programmation")
