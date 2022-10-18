MILESTONE = __file__[3]  # Should actually be "a", "b", "c"…

globals().update(__import__(f"workshop.en.{MILESTONE}",fromlist=["*"]).__dict__)

DISCLOSE_SECRET_WORD = TRUE

"""
For the sake of simplicity, the functions developed by the student in the
previous exercises use internal functions. Of course, the students will
use the code they wrote in the previous exercises.
"""


def pickWord(*args):
    return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


"""
- 'errorsAmount': the amount of errors.
Draw the part of the body corresponding to the amount of errors.
"""
def updateBody(errorsAmount):
  if errorsAmount == 1:
    bodyPart = P_HEAD
  elif errorsAmount == 2:
    bodyPart = P_TRUNK
  elif errorsAmount == 3:
    bodyPart = P_LEFT_ARM
  elif errorsAmount == 4:
    bodyPart = P_RIGHT_ARM
  elif errorsAmount == 5:
    bodyPart = P_RIGHT_LEG
  elif errorsAmount == 6:
    bodyPart = P_LEFT_LEG
  elif errorsAmount == 7:
    bodyPart = P_FACE

  if errorsAmount:
    drawBodyPart(bodyPart)


go(globals())
