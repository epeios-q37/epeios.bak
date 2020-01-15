# coding: utf-8
"""
MIT License

Copyright (c) 2017 Hajime Nakagami<nakagami@gmail.com>
Copyright (c) 2020 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import sys,os,time

sys.path.append("./term2web")

if ('Q37_XPP' in os.environ):
  sys.path.append(os.path.join(os.environ["HOME"],"epeios/other/libs/term2web/PYH/term2web"))


from term2web import *

import builtins

p = builtins.print

props = {}

def print_props(props):
    reset_style()

    print()

    for prop in props:
        print( prop + ": " + props[prop])
        globals()["props"][prop] = props[prop]

    set_style(globals()["props"])


def show(props):
    set_style(globals()["props"])

    print_props(props)

    print("Lorem ipsum dolor sit amet…")

    p(1)
    time.sleep(.75)
    p(2)


show({
    "text-decoration-line": "line-through"
})

show({
    "text-decoration-style": "wavy",
    "text-decoration-color": "red"
})

show({
    "color": "white",
    "background-color": "blue",
})

show({
    "text-decoration-line": "none",
    "text-shadow": "0 0 3px #FF0000, 0 0 5px #0000FF",
    "color": "initial",
    "background-color": "lightblue",
})

show({
     "font-style": "italic",
    "font-size": "larger"
})


show({
    "font-weight": "bold",
})
