The *Atlas* toolkit is particularly well suited for educational purposes, to write modern programming exercises, i.e. with a true graphical interface instead of the usual outdated textual one. More about this can be found [here](https://q37.info/s/cbms43s9).

*Python* is much more powerful then *Excel* macros to automate (boring) tasks, and you can also work with *PDF*, *Word*, *Google* files…. And with the *Atlas* toolkit, you have much more possibilities then with *VBA* forms. There are some examples [here](https://q37.info/s/97p44nh4).  

There is also a stub to for this library at address <https://q37.info/s/zzcn3wnx>.

> You will also find programs from [*The Big Book of Small Python Projects*](https://inventwithpython.com/bigbookpython/) by [Al Sweigart](http://alsweigart.com) to which a graphical user interface using the *Atlas* toolkit were added in this repository: [epeios-q37/AlSweigartTheBigBookPython](https://q37.info/s/kd3bwchj).

## Content of the repository

The `atlastk` directory contains the *Python* source code of the *Atlas* toolkit, which is the directory you have to reference in `PYTHONPATH` in order to use the *Atlas* toolkit in your own program, unless you have installed the [*atlastk* package](http://q37.info/s/9srmskcm) (`pip install atlastk`…).

You can also retrieve the `atlastk.zip` file, and add to your source code :

```python
__import__("sys").path.append("<path to>/atlastk.zip")

import atlastk
```

If the `atlastk.zip` file is in the current folder, replace `<path-to>/atlastk.zip` with `./atlastk.zip` and not only `atlastk.zip`.

In the `examples` directory, you will found following examples:

- `Blank`: very basic example,
- `Hello`: ["*Hello, World!*"](https://en.wikipedia.org/wiki/%22Hello,_World!%22_program),
- `Chatroom`: multi-user chatroom,
- `Notes`: note taking program,
- `TodoMVC`: [*TodoMVC*](http://todomvc.com/),
- `Hangman`: [Hangman game](http://q37.info/s/gtdtk4hp),
- `15-puzzle`: [15-puzzle game](https://q37.info/s/jn9zg3bn),
- `Contacts`: a basic address book, 
- `Widgets`: some widgets handled with the *Atlas* toolkit,
- `Chatrooms` : same as above `Chatroom`, but with several rooms,
- `PigGame`: [Pig game](https://en.wikipedia.org/wiki/Pig_(dice_game)) for one or two players,
- `Reversi`: [*Reversi* game](http://q37.info/s/zz3dzmf7) for one or two players,
- `MatPlotLib` : the *Atlas* toolkit displaying some graphics made with [*matplotlib*](https://matplotlib.org/); this example needs, of course, the *matplotlib* package to be installed…

Other examples are detailed in the next section.

Except for the *ErgoJr*, *GPIO* and *RGB* applications, which are detailed in the next section, to run an example, launch, from within the `examples` directory, `python <Name>/` (don't forget the final `/`), where `<Name>` is the name of the example (`Blank`, `Chatroom`…).

The *Stars* application is an example where the *Atlas* *toolkit* is used to control a [*Pygame*](https://en.wikipedia.org/wiki/Pygame) based application. Of course, *Pygame* needs to be installed.

The `tutorials` directory contains some [*Jupyter* notebooks](https://en.wikipedia.org/wiki/Project_Jupyter#Jupyter_Notebook) about  the *Atlas* *toolkit*.

## *Raspberry Pi*/*ODROID-C2*

**If the applications does not work on your *Raspberry Pi*, please see this issue: <https://github.com/epeios-q37/atlas-python/issues/1>**

The *GPIO* and *RGB* applications are designed to be used on a *Raspberry Pi* or a *ODROID-C2*.

Here is how the *WebGPIO* application looks like:

![*WebGPIO* interface](https://q37.info/s/htkhqb9x.png)

For the *Raspberry Pi*, the `RPi.GPIO` *Python* module have to be installed (this is probably already the case).

For the *ODROID-C2*, The *Python* version of *WiringPi* must be installed, and the application has to be launched, from within the `examples` directory, with `sudo` (`sudo python GPIO/` or `sudo python RGB/`).

The *ErgoJr* application is experimental and to control a *Poppy* *Ergo Jr* robot.

The *RGB* application is dedicated to the control of a RGB led, and the *GPIO* (aka *WebGPIO*) application allows to control the basic pins. Click below picture to see a *YouTube* video on how they work (same video on [*PeerTube*](https://en.wikipedia.org/wiki/PeerTube): <https://q37.info/s/49pbmwv9>):

[![RGB video](https://img.youtube.com/vi/C4p2iX6gc-Q/0.jpg)](https://www.youtube.com/watch?v=C4p2iX6gc-Q)

