# Modern exercises for a programming tutorial based on the [Hangman game](https://q37.info/s/gtdtk4hp)

- **This repository is (currently) a proof of concept,**
- **It is not finished (contributions welcome)**,
- **some exercises should perhaps be permutated and would probably be in the final version,**

**but it is fully functional.**

This project shows a new type of exercises for programming tutorials. It only contains the exercises, not (yet?) a full tutorial.

Each exercise consists in the programming of a functionality of the [Hangman game](https://q37.info/s/gtdtk4hp). When the last exercise is reached, the game will be fully operational.

Unlike other exercises which usually come with programming tutorials, this exercises will have a web interface, instead of the usual text-based one.

When a student launches an exercise to test the function which s/he has implemented, a web browser will be opened to display the corresponding interface, which will be used to test the correctness of the exercise.

As the last exercise, which will implement all the functionalities of the game, is not currently available, to see how the finished game looks like, go to <https://q37.info/s/jtdqjsx7>.

The `en` folder contains the English version of the exercises, and the `fr`, the French one.

If you want to adapt the exercises for another language, create a sub-folder in the `workshop` folder, like the `en` or `fr` folder, pick the content of one of this folder, and adapt this content to the desired language.

The content of the `en` or `fr` folder in the root folder must **not** be provided to the students. Only the `workshop` folder have to, with both ZIP files.

The students will begin with following content:

English version:

```python
# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.a import *

def isLetterInWord(letter,word):

go(globals())

```

French version (notice the `fr` in the `from workshop.fr.a import *` line):

```python
# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.a import *

def lettreEstDansMot(lettre, mot):

go(globals())
```

The exercise consists in implementing the `isLetterInWord(…)` (*fr*: `lettreEstDansMot(…)`) function. To explain to the students what the function must do, and what they will need for this, one can look to the (`en`|`fr`)`/a.py` file, which contains a description of the function, and also an example of code.

Once this exercise is completed, you tell the students to replace the `a` in `from workshop.en.a import *` (*fr*: `from workshop.fr.a import *`) with a `b` to obtain `from workshop.en.b import *` (*fr*: `from workshop.fr.b import *`). You also tell them to add following function prototype:

English:

```python
def getMask(word,guesses,dev)
```

French:

```python
def obtenirMasque(mot,pioches,dev)
```

This is how the new file looks like:

English:

```python
# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.b import *

def isLetterInWord(*args):
# What the student wrote in the previous exercise.

def getMask(word, guesses, dev):

go(globals())
```

French:

```python
# coding: utf-8

import sys
sys.path.append(".")
from workshop.fr.b import *

def lettreEstDansMot(*args):
# What the student wrote in the previous exercise.

def obtenirMasque(mot, pioches, dev):

go(globals())
```

To tell what is the task of the new function, and what it is needed for the implementation, you may also look at the (`en`/`fr`)`/b.py` file.

And so on for the *c*, *d*, *e*… exercises (when they will be available).

Between exercises, you will probably have to introduce the concepts that will be useful for the new exercise. For these, there are actually another set of exercises in development, which can be find here: <https://q37.info/s/tpkx4cfk>
