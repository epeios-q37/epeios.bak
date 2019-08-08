# Modern exercises for programming tutorials, based on the [Hangman game](https://q37.info/s/gtdtk4hp)

[![Hangman](https://q37.info/s/pnmjfw39)](https://q37.info/s/jtdqjsx7)

- **This repository is (currently) a proof of concept,**
- **It is under development (contributions welcome)**,
- **some exercises should perhaps be permutated and would probably be in the final version,**
- **but it is fully functional.**

This project shows a new type of exercises for programming tutorials. It only contains the exercises, not (yet?) a full tutorial.

Each exercise consists in the programming of a functionality of the [Hangman game](https://q37.info/s/gtdtk4hp). On the last exercise, the game will be fully operational. The finished game can be see at this address: <https://q37.info/s/jtdqjsx7>.

Other exercises provided with programming tutorials have almost always a text-based interface. The main point of this project is to provide exercises having all a web interface, that looks more like what young people these days are accustomed to with their smartphones.

When one of the exercises of this project is launched, a web browser will automatically be launched displaying the corresponding interface, which looks like the one in above picture. To facilitate the testings, there will also be a textbox which displays the word to guess, and in which one can also type the word to guess. Of course, this textbox will not be available in the final version of the game.

The application corresponding to the exercise could also be opened on any device connected to Internet, by providing the URL opened by the browser. This is facilitated by the [QR code](https://q37.info/s/3pktvrj7) displayed in the web browser, which can be scanned by a smartphone.

The `en` folder contains the English version of the exercises, and the `fr`, the French one. To adapt the exercises for another language, create a sub-folder in the `workshop` folder, like the `en` or `fr` folder, pick the content of one of this folder, and adapt this content to the desired language.

The content of the `en` or `fr` folder at the root folder must **not** be provided to the students, as they contain an example of the solution of the exercises, and are intended for the teacher to help the teachers to tell the students what they need to complete the exercises. Only the `workshop` folder have to be provided, along with both ZIP files.

To begin, the students will use a file following content:

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

The exercise consists in implementing the `isLetterInWord(…)` (*fr*: `lettreEstDansMot(…)`) function. As mentioned above, the teacher will use the content of the (`en`/`fr`)`/a.py` file to give to the students all the indication they need to achieve the exercise.

Once this exercise is completed, the students will be told to replace the `a` in `from workshop.en.a import *` (*fr*: `from workshop.fr.a import *`) with a `b` to obtain `from workshop.en.b import *` (*fr*: `from workshop.fr.b import *`). You also tell them to add following function prototype:

English:

```python
def getMask(word,guesses)
```

French:

```python
def obtenirMasque(mot,pioches)
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
