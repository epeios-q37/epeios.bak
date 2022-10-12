ETAPE = __file__[3]  # Devrait normalement être "a", "b", "c"…

globals().update(__import__(f"workshop.fr.{ETAPE}",fromlist=["*"]).__dict__)

DIVULGUER_MOT_SECRET = VRAI


def choisirMot(*args):
  return workshop.rfPickWord(*args)


def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)


def donnerMasque(*args):
    return workshop.rfGetMask(*args)


def majCorps(*args):
  return workshop.rfUpdateBody(*args)


"""
Ajout de la gestion du mot à deviner ('motSecret').
"""
class Pendu:
  def raz(self,suggestion,motAuHasard):
    self.motSecret = choisirMot(suggestion,motAuHasard)
    self.bonnesPioches = ""
    self.nombreErreurs = 0

  def __init__(self):
    self.motSecret = ""
    self.bonnesPioches = ""
    self.nombreErreurs = 0
    
  def traiterEtTesterPioche(self,pioche):
    if lettreEstDansMot(pioche,self.motSecret):
      if not lettreEstDansMot(pioche,self.bonnesPioches):
        self.bonnesPioches += pioche
      return VRAI
    else:
      self.nombreErreurs += 1
      return FAUX


"""
Utilisation du mot à deviner stocké dans 'pendu' ('pendu.motSecret').
Divulgation ou non du mot à deviner selon configuration.
"""
def raz(pendu,suggestion,motAuHasard):
  pendu.raz(suggestion,motAuHasard)
  print(pendu.motSecret)
  afficher(donnerMasque(pendu.motSecret,""))

  if DIVULGUER_MOT_SECRET:
    divulguerMotSecret( pendu.motSecret )
  

def traiterPioche(pendu,pioche):
  if pendu.traiterEtTesterPioche(pioche):
    afficher(donnerMasque(pendu.motSecret,pendu.bonnesPioches))
  else:
    majCorps(pendu.nombreErreurs)

go(globals())
