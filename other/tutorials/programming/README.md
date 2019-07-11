# Programming tutorial

*This tutorial support in currently under development.*

This is a modern, powerful and easy to install support for a programming tutorial. Although written in *Python*, it's __not__ for learning *Python*.

It's only a support, not a complete tutorial. It's intended for those who want to teach programming, so their students can write programs, to practice, which are far more interactive, therefor more interesting, then those based on the traditional *print(…)*, *input(…)* and similar instructions.

Although very powerful, it remains lightweight and easy to install thanks to the [*Atlas* toolkit](http://atlastk.org) (<http://atlastk.org>).

Simply retrieve the repository (`git clone …` or load and unpack the zip file) and go, in a console, to the root directory. Then, to launch an exercise: `python (de|en|fr)/(A|B|C|Z_(1|2|3)).py`. For example: `python fr/B.py`.

The *Z_…* exercises are currently only available for *fr*.

The `workshop/assets` directory contains files used for the exercises. The `ab` sub-directory is for exercises *A* and *B*, the `c`, `z_1`, `z_2` and `z_3 ` sub-directories are respectively for the *C*, *Z_1*, *Z_2* and *Z_3* exercises (the *Z_…* exercises are for *fr* only).

The content of the `Body.html` will be put in the *body* section of the HTML main page, and the content of the `Head.html` file will be put in the *head* section of the same page. The CSS rules are usually put in the latter file.

- exercise *A*: introducing strings and function call,
- exercise *B*: introducing variables and strings concatenation,
- exercise *C*: introducing function definition (interactive).

Below exercises are currently only available for *fr*.

- exercise *Z_1*: (in)equation of the first degree (interactive),
- exercise *Z_2*: example of use of turtle graphics,
- exercise *Z_3*: drawing of polygons using turtle graphics (interactive).

The `main.py` and `starter.py` files are mainly to use with *repl.it*.
