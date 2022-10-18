MILESTONE = "a"

globals().update(__import__(f"workshop.en.{MILESTONE}",fromlist=["*"]).__dict__)

"""
If missing or set to 'FALSE' or 'False', the text box displaying
the secret word, and allowing to type its value, 
will not be displayed (default mode).
"""
DISCLOSE_SECRET_WORD = TRUE


"""
Returns any word.
"""
def pickWord():
  pass  # Replace with your code.


go(globals())
