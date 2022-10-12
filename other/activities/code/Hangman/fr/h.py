ETAPE = __file__[3]  # Devrait normalement être "a", "b", "c"…

globals().update(__import__(f"workshop.fr.{ETAPE}",fromlist=["*"]).__dict__)

DIVULGUER_MOT_SECRET = VRAI


def choisirMot(*args):
  return workshop.rfPickWord(*args)


def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)


def donnerMasque(*args):
    return workshop.rfGetMask(*args)
    

"""
'P_VISAGE' est volontairement omis (voir fonction suivante).
"""
PARTIES_CORPS = (
  P_TETE,
  P_TRONC,
  P_BRAS_GAUCHE,
  P_BRAS_DROIT,
  P_PIED_GAUCHE,
  P_PIED_DROIT,
#  P_VISAGE
)


"""
- 'nombreErreurs': nombre d'erreurs.
Dessiner la partie du corps correspondant au nombre d'erreurs.
Lorsque la dernière partie du corps est dessinée, le visage ('P_VISAGE') doit
également être dessiné.
"""
def majCorps(nombreErreurs):
  dessinerPartieCorps(PARTIES_CORPS[nombreErreurs-1])

  if nombreErreurs >= len(PARTIES_CORPS):
    dessinerPartieCorps(P_VISAGE)


go(globals())
