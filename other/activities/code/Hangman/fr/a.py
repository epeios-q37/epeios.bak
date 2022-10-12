ETAPE = __file__[3]  # Devrait normalement être "a", "b", "c"…

globals().update(__import__(f"workshop.fr.{ETAPE}",fromlist=["*"]).__dict__)

"""
Si absent ou mis à 'False', le champ texte
affichant le mot secret et permettant sa saisie
ne sera pas affiché (mode par défaut).
"""
DIVULGUER_MOT_SECRET = VRAI


"""
Retourne un mot quelconque.
"""
def choisirMot():
  return "pied"


go(globals())
