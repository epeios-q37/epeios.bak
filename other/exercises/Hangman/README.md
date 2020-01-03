
<!--
    For Repl.it users: click on the green 'run' button,
    select an exercise, and click on (or scan) the then
    displayed QR code.
-->

# Modern programming exercises, featuring the [Hangman game](https://q37.info/s/gtdtk4hp).

*Enseignants (French-speaking teachers)* : notice en français pour utiliser ces exercices en cours : <https://q37.info/s/mmdnch9t>.

[![Run on Repl.it](https://repl.it/badge/github/epeios-q37/hangman-exercises)](https://q37.info/s/z3whmbfz) [![License](https://img.shields.io/pypi/l/atlastk.svg?style=plastic)](https://github.com/epeios-q37/hangman-exercises/blob/master/LICENSE)

This project shows an example of modern programming exercises, featuring the [Hangman game](https://q37.info/s/gtdtk4hp). See <https://q37.info/s/cbms43s9> for the rationale behind this project.

[![Hangman](https://q37.info/s/pnmjfw39)](https://q37.info/s/jtdqjsx7)

Simply retrieve the repository (`git clone https://github.com/epeios-q37/hangman-exercises`, or get the corresponding *ZIP* file here: <https://q37.info/s/f7mt4s7n>), and, in a console, in the root directory of the repository, launch `python main.py` (you can change the language of the exercises by modifying `main.py`). You can also specifically launch an exercise with `python (en|fr)/(a|b|…|h|i).py` (for example: `python fr/b.py`). You can (should) also use `python3` instead of `python`.

Alternatively, you can also use [*Repl.it*](https://q37.info/s/srnnb7hj), so you have nothing to install on your computer. Follow this [link](https://q37.info/s/z3whmbfz), click on the green `run` button, select an exercise, and then click on (or scan) the then displayed [QR code](https://q37.info/s/3pktvrj7). To choose another exercise, click on the green `restart` button.

Each exercise consists in programming a feature of the [Hangman game](https://q37.info/s/gtdtk4hp). With the last exercise, the game will be fully operational. The finished game can be see at this address: <https://q37.info/s/jtdqjsx7>.

When one of the exercises is launched, a web browser will automatically be opened to give access to its interface, like the one you can see in the above picture. To make it easier for students to test the code they wrote, there is also a text box that will display the word to guess, and in which you can also enter the word to guess. If you don't give a word, a random word will be used. Of course, this text box will not be available in the final version of the game.

The application for a given exercise can also be opened on any device with a modern web browser connected to the Internet, by using the URL displayed in the browser. This is facilitated by the [QR code](https://q37.info/s/3pktvrj7) displayed in the web browser, which can be scanned with, for example, a smartphone.

The `en` folder contains the English version of the exercises, and the `fr`, the French one. To adapt the exercises to another language, create a sub-folder in the `workshop` folder, like the `en` or `fr` folder, retrieve the content of one of this folder, and adapt this content to the desired language.

The content of the `en` or `fr` folder located in the root folder should **not** be provided to the students, as they contain an example of solution for the exercises, and are intended to help the teachers to give to their students all the information they need to complete the exercises. Only the `workshop` folder should be provided, along with both ZIP files.

To begin, the students will create a file with following content:

English version:

```python
# coding: utf-8

from workshop.en.a import *

SHOW_SECRET_WORD = TRUE

def pickWord(suggestion):

go(globals())
```

French version (note the `fr` in the `from workshop.fr.a import *` line):

```python
# coding: utf-8

from workshop.fr.a import *

MONTRER_MOT_SECRET = VRAI

def choisirMot(suggestion):

go(globals())
```

The first exercise consists in implementing the `pickWord(…)` (*fr*: `choisirMot(…)`) function. As mentioned above, the teacher will use the content of the `(en|fr)/a.py` file to give to the students all the instructions they need to complete the exercise.

Once this first exercise is completed, the students will be instructed to replace the `a` in `from workshop.en.a import *` (*fr*: `from workshop.fr.a import *`) with a `b` to obtain `from workshop.en.b import *` (*fr*: `from workshop.fr.b import *`). To tell the students what to do, the teachers will also look at the `(en|fr)/b.py` file.

And so on for the *c*, *d*, *e*… exercises.

---

This project is based on the [*Atlas* toolkit](https://atlastk.org). Other projects using this toolkit can be found here: <https://q37.info/s/sssznrb4>.
