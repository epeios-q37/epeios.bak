ETAPE = __file__[3]  # Devrait normalement être "a", "b", "c"…

globals().update(__import__(f"workshop.fr.{ETAPE}",fromlist=["*"]).__dict__)

DIVULGUER_MOT_SECRET = VRAI


"""
- 'motAuHasard': un mot aléatoire ;
- 'suggestion'; le contenu du champ texte du mot secret ;
  utilisé seulement en mode 'dev'.
Retourne 'suggestion' si non vide, 'motAuHasard' sinon.
"""
def choisirMot(suggestion,motAuHasard):
  if suggestion:
    return suggestion
  else:
    return motAuHasard


go(globals())
