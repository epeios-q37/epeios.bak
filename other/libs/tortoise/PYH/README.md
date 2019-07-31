# Tortoise: [turtle graphics](https://q37.info/s/3dwhcdfm) on the web in *Python*

Cloning the repository (or retrieving corresponding *ZIP* file) will suffice to use this module. The `main.py` file is an exemple of use of this library. Simply launch `python main.py` from the root of the repository.

The module is also available from *Pypi*: <https://pypi.org/project/tortoise/>. Simply launch `pip install tortoise`.

You can also test the library with nothing to install by using [*Repl.it*](https://q37.info/s/mxmgq3qm):

- Go to <https://q37.info/s/srnnb7hj>,
- click on he `+ new repl` button,
- select *Python* as language,
- put the address of this repository (<https://github.com/epeios-q37/tortoise-python> in the dedicated field,
- click on the ``Create repl`` button,
- click on the green button, and then on the [QR code](https://q37.info/s/3pktvrj7) to launch the example.

Available methods:

- `getAngle()`: returns the current angel,
- `up()`: put the pen up,
- `down()`: puts the pen down,
- `setAutoDraw(value)`: the draw is automatically rendered each `values` movements; `0` for disabling; call `draw()` to render remaining movements,
- `setPosition(x,y)`: the turtle jumps, without drawing, to `x`, and `y`, relatively of the center of the drawing area,
- `forward(distance)`: the tortoise moves by the specified `distance`,
- `right(angle)`: the tortoise turns clockwise from `angle` degrees,
- `right(angle)`: the tortoise turns *counter* clockwise from `angle` degrees,
- `setColorRGB(r,g,b)`: sets the color of the pen following the RGB color model,
- `setColorHSL(h,s,l)`: sets the color of the pen following the HSL color model, (`h`: 0 - 360; `s`, `l`: 0 - 100%),
- `draw()`; draws remaining movements.

