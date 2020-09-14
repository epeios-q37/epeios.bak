
# Tortoise: [turtle graphics](https://q37.info/s/3dwhcdfm) on the web

[![Run on Repl.it](https://q37.info/s/kpm7xhfm.png)](https://q37.info/s/p3cvfxdx) [![About online demonstrations](https://img.shields.io/badge/about-online%20demonstrations-informational)](https://q37.info/s/sssznrb4)

[![license: MIT](https://img.shields.io/github/license/epeios-q37/tortoise-python?color=yellow&style=for-the-badge)](https://github.com/epeios-q37/tortoise-python/blob/master/LICENSE) [![Version](https://img.shields.io/pypi/v/tortoise?style=for-the-badge&color=90b4ed&label=PyPi)](http://q37.info/s/k3c3fmvp)


[![tortoise library example](https://q37.info/s/frgzbhq9)](http://q37.info/s/dj9b7ksf)

This repository is the [proof of concept](https://q37.info/s/hzpbtv7r) of an implementation, in *Python*, of [turtle graphics](https://q37.info/s/3dwhcdfm) based on web technologies. This is the main difference with the *turtle* module, which uses *(t|T)kinter*.

Cloning the repository (or retrieving the corresponding ZIP file) is sufficient to use this module. The `main.py` file, with which above picture were generated, is an example of use of this library. Simply launch `python main.py` from the root of the repository.

The module is also available on *Pypi*: <https://pypi.org/project/tortoise/>. Simply launch `pip install tortoise` to install it.

Alternatively, you can also use [*Repl.it*](https://q37.info/s/mxmgq3qm), so you have nothing to install on your computer. Follow this [link](https://q37.info/s/p3cvfxdx), click on the green `run` button, and then click on (or scan) the then displayed [QR code](https://q37.info/s/3pktvrj7).

This *Python* module is part of a [larger project](https://q37.info/s/tpkx4cfk) that aims to provide a way to write modern examples or exercises for programming tutorials.

The *tortoise* library currently provides following methods (coordinates and distances are relative to the *viewbox* of the SVG):

- *constructor*`(dom,id)`: returns a *tortoise* object which uses `dom` as the *DOM* from the [*Atlas* toolkit](https://q37.info/s/c7hfkzvs), and the SVG element of id `id`,
- `getAngle()`: returns the current angel, in degrees,
- `up()`: puts the pen up,
- `down()`: puts the pen down,
- `setAutoDraw(value)`: the drawing is automatically rendered each `value` movements (`0` will disable the auto draw); call `draw()` to render remaining movements,
- `setPosition(x,y)`: the turtle jumps to position `x` and `y`,
- `forward(distance)`: the tortoise moves by the specified `distance`,
- `right(angle)`: the tortoise turns clockwise from `angle` degrees,
- `left(angle)`: the tortoise turns *counter* clockwise from `angle` degrees,
- `setColorRGB(r,g,b)`: sets the color of the pen following the RGB color model,
- `setColorHSL(h,s,l)`: sets the color of the pen following the HSL color model, (`h`: 0 - 360; `s`, `l`: 0 - 100%),
- `draw()`: draws remaining movements,
- `clear()`: erase the drawing area and also resets the internal settings (coordinates, angle, color…).
