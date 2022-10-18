MILESTONE = __file__[3]  # Should actually be "a", "b", "c"…

globals().update(__import__(f"workshop.en.{MILESTONE}",fromlist=["*"]).__dict__)

DISCLOSE_SECRET_WORD = TRUE

"""
Some variables are now handled by the student. Names are free.
"""


"""
Can be omitted, as 'reset(…)' will be call before the variables
will be used.
"""
goodGuesses = ""
errorsAmount = 0


"""
NOTA: the four folowing functions are not called outside this file.
"""


def pickWord(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


def getMask(*args):
    return workshop.rfGetMask(*args)


def updateBody(*args):
  return workshop.rfUpdateBody(*args)


"""
Reset the variables and the display for a new round and
return the secret word.
"""
def reset(suggestion,randomWord):
  global goodGuesses,errorsAmount

  secretWord = pickWord(suggestion,randomWord)
  goodGuesses = ""
  errorsAmount = 0
  print(secretWord)
  display(getMask(secretWord,""))

  return secretWord


"""
N.B.: NOT THREAD-SAFE!!!
Multiple instances can be launched to show
why this is a problem.
"""
"""
- 'guess': the letter chosen by the player,
If 'guess' in 'word', must update the mask, otherwise
must update the drawing of the body.
"""
def handleGuess(guess,secretWord):
  global goodGuesses,errorsAmount

  if isLetterInWord(guess,secretWord): # Test is not mandatory
    if not isLetterInWord(guess,goodGuesses):
      goodGuesses += guess
      display(getMask(secretWord,goodGuesses))
  else:
    errorsAmount += 1
    updateBody(errorsAmount)

go(globals())
