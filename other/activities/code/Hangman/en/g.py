MILESTONE = __file__[3]  # Should actually be "a", "b", "c"…

globals().update(__import__(f"workshop.fr.{MILESTONE}",fromlist=["*"]).__dict__)

DISCLOSE_SECRET_WORD = TRUE


def pickWord(*args):
    return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


"""
Alternative version which uses a tuple.
"""
BODY_PARTS = (
  P_HEAD,
  P_TRUNK,
  P_LEFT_ARM,
  P_RIGHT_ARM,
  P_LEFT_LEG,
  P_RIGHT_LEG,
  P_FACE 
)


"""
- 'errorsAmount': the amount of errors.
Draw the part of the body corresponding to the amount of errors.
"""
def updateBody(errorsAmount):
  drawBodyPart(BODY_PARTS[errorsAmount-1])


go(globals())
