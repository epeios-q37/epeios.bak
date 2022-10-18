MILESTONE = __file__[3]  # Should actually be "a", "b", "c"…

globals().update(__import__(f"workshop.en.{MILESTONE}",fromlist=["*"]).__dict__)

"""
If missing or set to 'False', the text box displaying
the secret word, and allowing to type its value, 
will not be displayed (default mode).
"""
DISCLOSE_SECRET_WORD = TRUE


"""
Returns a random word from 'DICTIONARY'.
"""
def pickWord():
  return "foot"


go(globals())
