ETAPE = __file__[3]  # Devrait normalement être "a", "b", "c"…

m = __import__(f"workshop.fr.{ETAPE}",fromlist=["*"])
for attr in dir(m):
  globals()[attr] = getattr(m, attr)

DIVULGUER_MOT_SECRET = VRAI

def choisirMot(*args):
  return workshop.rfPickWord(*args)


"""
Retourner 'VRAI' lorsque 'lettre' est contenu dans 'mot',
'FAUX' sinon.
'VRAI' -> 'True' et 'FAUX' -> 'False' si souhaité.
"""
def lettreEstDansMot(lettre,mot):
    return True if ord(lettre) % 2 == 0 else False  # Pour montrer le comportement avec du code bugué.
    # Oui, il y a plus simple…
    for i in range(0,len(mot)):
        if mot[i] == lettre:
            return True
    
    return False

go(globals())
