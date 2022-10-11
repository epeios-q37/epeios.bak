MILESTONE = __file__[3]  # Should actually be "a", "b", "c"…

m = __import__(f"workshop.en.{MILESTONE}",fromlist=["*"])
for attr in dir(m):
  globals()[attr] = getattr(m, attr)

DISCLOSE_SECRET_WORD = TRUE


"""
- 'randomWord': an random word,
- 'suggestion'; the content of the secrete word text box;
  used only in 'dev' mode.
Return 'suggestion', if not empty, otherwise 'randomWord'.
"""
def pickWord(suggestion,randomWord):
  if suggestion:
    return suggestion
  else:
    return randomWord


go(globals())
