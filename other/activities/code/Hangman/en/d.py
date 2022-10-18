MILESTONE = __file__[3]  # Should actually be "a", "b", "c"…

globals().update(__import__(f"workshop.en.{MILESTONE}",fromlist=["*"]).__dict__)

DISCLOSE_SECRET_WORD = TRUE

def pickWord(*args):
    return workshop.rfPickWord(*args)


"""
# Return 'TRUE' when 'letter' is in 'word', 'FALSE' otherwise.
# 'TRUE' -> 'True' and 'FALSE' -> 'False' at whish.
"""
def isLetterInWord(letter,word):
    return True if ord(letter) % 2 == 0 else False # To show behavior on buggy code.
    # Yes, there is a simpler way…
    for i in range(0,len(word)):
        if word[i] == letter:
            return True

    return False


go(globals())
