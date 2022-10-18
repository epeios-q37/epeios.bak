MILESTONE = __file__[3]  # Should actually be "a", "b", "c"…

globals().update(__import__(f"workshop.en.{MILESTONE}",fromlist=["*"]).__dict__)

DISCLOSE_SECRET_WORD = TRUE


def pickWord(*args):
  return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)


"""
Previously named 'getMask'.
Add the testing.
"""
def getMaskAndTestIfHasWon(word,guesses):
  mask = ""
  hasWon = True

  for letter in word:
    if isLetterInWord(letter,guesses):
      mask += letter
    else:
      mask += "_"
      hasWon = False

  return mask,hasWon


"""
Add the testing.
"""
def updateBodyAndTestIfHasLost(errorsAmount):
  workshop.rfUpdateBody(errorsAmount)    

  return errorsAmount >= (P_AMOUNT - 1)



"""
At first, do not handle the 'inProgress' variable
member from the 'Hangman' class.
"""


"""
Add the handling of the 'inProgress' variable.
Only the 'reset(…)' and the '__init__(…)'
methods are concerned.
"""
class Hangman:
  def reset(self,suggestion,randomWord):
    self.secretWord = pickWord(suggestion,randomWord)
    self.goodGuesses = ""
    self.errorsAmount = 0
    self.inProgress = TRUE

  def __init__(self):
    self.secretWord = ""
    self.goodGuesses = ""
    self.errorsAmount = 0
    self.inProgress = FALSE

  def handleAndTestGuess(self,guess):
    if isLetterInWord(guess,self.secretWord):
      if not isLetterInWord(guess,self.goodGuesses):
        self.goodGuesses += guess
      return TRUE
    else:
      self.errorsAmount += 1
      return FALSE



"""
Modify to use 'getMaskAndTestIfHasWon(…)'.
"""
def reset(hangman,suggestion,randomWord):
  hangman.reset(suggestion,randomWord)
  print(hangman.secretWord)
  display(getMaskAndTestIfHasWon(hangman.secretWord,"")[0])

  if DISCLOSE_SECRET_WORD:
    discloseSecretWord(hangman.secretWord)


"""
Add the notifications.
"""
def handleGuess(hangman,guess):
  if hangman.handleAndTestGuess(guess):
    mask,hasWon = getMaskAndTestIfHasWon(hangman.secretWord,hangman.goodGuesses)
    display(mask)
    if hasWon and hangman.inProgress:
      notify("You won! Congratulations!")
      hangman.inProgress = FALSE
  elif hangman.inProgress and updateBodyAndTestIfHasLost(hangman.errorsAmount):
    notify("\nYou lose!\nErrors: {}; good guesses: {}.\n\nThe secret word was: '{}'.".format(hangman.errorsAmount,len(hangman.goodGuesses),hangman.secretWord))
    hangman.inProgress = FALSE


"""
Called on new connection. 
"""
def AConnect(hangman,suggestion,randomWord):
  return reset(hangman,suggestion,randomWord)


"""
Called on new guess.
NOTA: the letter will be disabled on the keyboard. 
"""
def ASubmit(hangman,guess):
  handleGuess(hangman,guess)


"""
Called on a click on the 'Restart' button.
"""
def ARestart(hangman,suggestion,randomWord):
  if hangman.inProgress:
    notify("\nErrors: {}; good guesses: {}.\n\nThe secret word was: '{}'.".format(
        hangman.errorsAmount,len(hangman.goodGuesses),hangman.secretWord))

  return reset(hangman,suggestion,randomWord)


go(globals())
