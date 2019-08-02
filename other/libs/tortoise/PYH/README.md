# Tortoise: [turtle graphics](https://q37.info/s/3dwhcdfm) on the web

[![Version 0.1.0](https://img.shields.io/static/v1.svg?&color=90b4ed&label=Version&message=0.1.0)](https://q37.info/s/q4ghchsr) [![License](https://img.shields.io/pypi/l/atlastk.svg?style=plastic)](https://github.com/epeios-q37/tortoise-python/blob/master/LICENSE)

[![tortoise library example](https://q37.info/s/frgzbhq9)](http://q37.info/s/dj9b7ksf)

This repository is the [proof of concept](https://q37.info/s/hzpbtv7r) of an implementation, in *Python*, of [turtle graphics](https://q37.info/s/3dwhcdfm) based on web technologies. This is the main difference with the *turtle* module, which uses *(t|T)kinter*.

Cloning the repository (or retrieving the corresponding ZIP file) is sufficient to use this module. The `main.py` file, with which above picture were generated, is an example of use of this library. Simply launch `python main.py` from the root of the repository.

The module is also available on *Pypi*: <https://pypi.org/project/tortoise/>. Simply launch `pip install tortoise` to install it.

You can also test the library directly in your web browser, with nothing to install: <http://q37.info/s/dj9b7ksf>.

This *Python* module is part of a [larger project](https://q37.info/s/tpkx4cfk) that aims to provide a way to write modern examples or exercises for programming tutorials.

The *tortoise* library currently provides following methods:

- `getAngle()`: returns the current angel,
- `up()`: put the pen up,
- `down()`: puts the pen down,
- `setAutoDraw(value)`: the drawing is automatically rendered each `values` movements (`0` will disable the auto draw); call `draw()` to render remaining movements,
- `setPosition(x,y)`: the turtle jumps to `x` and `y`, relatively of the center of the drawing area,
- `forward(distance)`: the tortoise moves by the specified `distance`,
- `right(angle)`: the tortoise turns clockwise from `angle` degrees,
- `right(angle)`: the tortoise turns *counter* clockwise from `angle` degrees,
- `setColorRGB(r,g,b)`: sets the color of the pen following the RGB color model,
- `setColorHSL(h,s,l)`: sets the color of the pen following the HSL color model, (`h`: 0 - 360; `s`, `l`: 0 - 100%),
- `draw()`; draws remaining movements.

