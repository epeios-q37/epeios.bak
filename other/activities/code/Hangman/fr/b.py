ETAPE = __file__[3]  # Devrait normalement être "a", "b", "c"…

globals().update(__import__(f"workshop.fr.{ETAPE}",fromlist=["*"]).__dict__)

DIVULGUER_MOT_SECRET = VRAI


"""
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, un mot quelconque sinon.
"""
def choisirMot(suggestion):
  if len(suggestion) != 0:
    return suggestion
  else:
    return "bras"


go(globals())
