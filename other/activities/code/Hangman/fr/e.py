ETAPE = __file__[3]  # Devrait normalement être "a", "b", "c"…

m = __import__(f"workshop.fr.{ETAPE}",fromlist=["*"])
for attr in dir(m):
  globals()[attr] = getattr(m, attr)

DIVULGUER_MOT_SECRET = VRAI


def choisirMot(*args):
  return workshop.rfPickWord(*args)
  

def lettreEstDansMot(*args):
    return workshop.rfIsLetterInWord(*args)

"""
Retourner une chaîne de caractères contenant 'mot' (qui est en minuscule) où
chaque lettre qui n'est pas contenue dans 'pioches' (également en minuscules)
est remplacée par le caractère '_'.
"""
def donnerMasque(mot,pioches):
    masque = ""

    for lettre in mot:
        if lettreEstDansMot(lettre,pioches):
            masque += lettre
        else:
            masque += "_"

    return masque

go(globals())
