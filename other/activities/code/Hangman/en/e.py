MILESTONE = __file__[3]  # Should actually be "a", "b", "c"…

globals().update(__import__(f"workshop.en.{MILESTONE}",fromlist=["*"]).__dict__)

DISCLOSE_SECRET_WORD = TRUE


def pickWord(*args):
    return workshop.rfPickWord(*args)


def isLetterInWord(*args):
    return workshop.rfIsLetterInWord(*args)

"""
Return a string containing 'word' (which is in lowercase) where
all letters which are not in in 'guesses' (also in lowercase)
are replaced by the '_' character.
"""
def getMask(word,guesses):
    mask = ""

    for letter in word:
        if isLetterInWord(letter,guesses):
            mask += letter
        else:
            mask += "_"

    return mask

go(globals())
