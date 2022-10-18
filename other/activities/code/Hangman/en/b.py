MILESTONE = __file__[3]  # Should actually be "a", "b", "c"…

globals().update(__import__(f"workshop.en.{MILESTONE}",fromlist=["*"]).__dict__)

DISCLOSE_SECRET_WORD = TRUE


"""
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Return 'suggestion', if not empty, otherwise some word.
"""
def pickWord(suggestion):
  if suggestion:
    return suggestion
  else:
    return "arm"


go(globals())
