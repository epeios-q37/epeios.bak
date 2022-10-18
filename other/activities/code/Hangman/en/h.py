MILESTONE = __file__[3]  # Should actually be "a", "b", "c"…

globals().update(__import__(f"workshop.en.{MILESTONE}",fromlist=["*"]).__dict__)

DISCLOSE_SECRET_WORD = TRUE


def pickWord(*args):
    return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


"""
"P_FACE' is deliberately omitted (see next function).
"""
BODY_PARTS = (
  P_HEAD,
  P_TRUNK,
  P_LEFT_ARM,
  P_RIGHT_ARM,
  P_LEFT_LEG,
  P_RIGHT_LEG,
#  P_FACE 
)


"""
- 'errorsAmount': the amount of errors.
Draw the part of the body corresponding to the amount of errors.
When the last part of the body is drawn, the face ('P_FACE') must
also be drawn.
"""
def updateBody(errorsAmount):
  drawBodyPart(BODY_PARTS[errorsAmount-1])

  if errorsAmount >= len(BODY_PARTS):
    drawBodyPart(P_FACE)


go(globals())
