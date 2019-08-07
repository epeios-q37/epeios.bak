# Exercises for a programming tutorial based on the [Hangman game](https://q37.info/s/gtdtk4hp)

The `en` folder contains the English version of the tutorial, and the `fr`, the French one.

If you want to adapt the tutorial for another language, you have to crate a sub-folder in the `workshop` folder, like the `en` and `fr`, folder, pick the content of one of this folder, and adapt this content to the desired language.

The content of the `en` or `fr` folder in the root folder must **not** be provided to the student. Only the `workshop` folder.

For the beginning, the student begins with following content:

English version:

```python
# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.a import *

def isLetterInWord(letter,word):

go(globals())

```

French version (notice the `fr` in the `from workshop.fr.a import *` instruction):

```python
# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.a import *

def lettreEstDansMot(lettre, mot):

go(globals())
```

The exercise consist to implement the `isLetterInWord(…):` (*fr*: `lettreEstDansMot(…)`). To explain to student what they have to do, see the (`en`|`fr`)`/a.py` file.

Once this exercise completed, you tell the student to modify the `a` in `from workshop.en.a import *` (*fr*: `from workshop.fr.a import *`) to obtain `from workshop.en.b import *` (*fr*: `from workshop.fr.b import *`). You also tell the to add following function prototype:

English:

```python
def displayMask(word,guesses,dev)
```

French:

```python
def afficheMasque(mot,pioches,dev)
```

To obtain this.
