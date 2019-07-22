# Programming tutorial

*Project currently under development. Contributions are welcome.*

This project is about how to write modern exercises or examples for programming tutorials. This exercises/examples have a graphical user interface instead of the usual textual interface, which are less and less used, or even known, by those who are mainly using smartphones.

Simply retrieve the repository (`git clone …`, or get the corresponding *ZIP* file here: <https://q37.info/s/js7fm3vj>) and go, in a console, to the root directory. Then, to launch an exercise: `python (en|fr)/(A|B|C|Z_(1|2(a|b)|3)).py`. For example: `python fr/B.py`.

Alternatively, you can also clone this repository on [*Repl.it*](https://q37.info/s/srnnb7hj), so you have nothing to install on your computer. Click on the *new repl* button, select *Python* as language, and put the address of this repository in the dedicated field. The `main.py` and `starter.py` files are especially for *Repl.it*.

The files found under the ``en`` and ``fr`` root directories are examples of what the students have to figure out.

The *A*, *B* and *C* exercises are variations of the famous [*Hello, World!*](https://q37.info/s/k9hfpjbq) program, and are intended to familiarize the student with strings, string concatenation, function calling and definition… Exercise *C* is interactive.

The *Z* series of exercises deals with more advanced programming concepts.

The *Z_1* exercise deals with the solving of first-degree (in)equations. The student has to find out how to calculate the solutions of such an (in)equation, and can test if its code works properly through a convenient user interface, without having to program this interface.

The *Z_2a* and *Z_2b* exercises are example of use of the [turtle graphics](https://q37.info/s/3dwhcdfm) API. Exercise *Z_2b* is interactive.

The purpose of the *Z_3* exercise is to program the [hangman game](https://q37.info/s/gtdtk4hp). As for the other exercises, the student handles the user interface by using very simple functions and can therefore focus on the handling of the inputs from the user.

The `workshop/assets` directory contains files used for the exercises. The `ab` sub-directory is for exercises *A* and *B*, the `c`, `z_1`, `z_2a`… respectively for the exercises *C*, *Z_1*, *Z_2a*… The content of the `Body.html` files will be put in the *body* section of the HTML main page, and the content of the `Head.html` files will be put in the *head* section of the same page. CSS rules are usually put in the latter file.

For the displaying, here are the currently available functions:

English | Français
-|-
erase | efface
display | affiche
eraseAndDisplay | effaceEtAffiche
warn | alerte
ask | demande

For the tutle graphics, here are the currently available functions:

English | Français
-|-
| forward | avance 
| turnRight | tourneDroite
| turnLeft | tourneGauche
| setColorRGB | fixeCouleurRVB
| setColorHSL | fixeCouleurTSL
