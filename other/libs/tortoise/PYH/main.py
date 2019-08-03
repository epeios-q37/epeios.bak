# coding: utf-8
"""
MIT License

Copyright (c) 2019 Claude SIMON (https://q37.info/s/rmnmqd49)

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

import os, sys
sys.path.append("./Atlas.python.zip")
sys.path.append("./tortoise")

# For using in Repl.it
if ('HOME' in os.environ) and (os.environ['HOME'] == '/home/runner'):
  os.environ["ATK"] = "REPLit"

import atlastk
from tortoise import *

svgViewBoxScript = """
function svgViewBox( id ) {
 let x = 0;
 let y = 0;
 let width = 0;
 let height = 0;
 let svg = document.getElementById(id);
 let viewBox = svg.viewBox.baseVal;

 if ( viewBox !== null ) {
  x = viewBox.x;
  y = viewBox.y;
  width = viewBox.width;
  height = viewBox.height;
}

 if ( height === 0 ) {
  if ( width !== 0 )
   return "";
 } else if ( width === 0 ) 
  return ""

 if ( width === 0 ) {
  x = 0;
  y = 0;
  width = svg.width.baseVal.value;         
  height = svg.height.baseVal.value;         
 }

  return x.toString() + " " +  y.toString() + " " + width.toString() + " " + height.toString();
}

svgViewBox('SVG');
"""


HTML = """
<div style="display: table; margin: auto;">
 <svg id="SVG" width="200" height="300">
  <text x="30" y="30">Click on a button below!</text>
 </svg>
 <div id="buttons" style="display: table; margin: 10px auto auto auto;">
  <button id="All" data-xdh-onevent="All">All</button>
  <button id="0" data-xdh-onevent="Draw">1</button>
  <button id="1" data-xdh-onevent="Draw">2</button>
  <button id="2" data-xdh-onevent="Draw">3</button>
  <button id="3" data-xdh-onevent="Draw">4</button>
  <button id="4" data-xdh-onevent="Draw">5</button>
 </div>
</div>
"""

HEAD = """
<title>Turtle graphics with the Atlas toolkit</title>
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
<style>
 #SVG {
  border: 1px solid;
  padding: 10px;
  box-shadow: 5px 5px;
  display: table;
  margin: auto;
  text-align: center;
 }
</style>
"""

autoDraw = 20
ids = ["All", "0", "1", "2", "3", "4"]


def koch(tortoise, l):
    if l < 3:
        tortoise.setColorHSL(tortoise.getAngle(), 100, 50)
        tortoise.forward(l*2)
    else:
        koch(tortoise, l/3.0)
        tortoise.left(60)
        koch(tortoise, l/3.0)
        tortoise.right(120)
        koch(tortoise, l/3.0)
        tortoise.left(60)
        koch(tortoise, l/3.0)


def drawing0(tortoise):
    tortoise.setPosition(-65, 85)
    tortoise.down()

    for _ in range(3):
        koch(tortoise, 110)
        tortoise.right(120)


def drawing1(tortoise):
    tortoise.down()

    for i in range(180):
        tortoise.setColorHSL(i * 2, 100, 50)
        tortoise.forward(100)
        tortoise.right(60)
        tortoise.forward(20)
        tortoise.left(120)
        tortoise.forward(50)
        tortoise.right(60)

        tortoise.up()
        tortoise.setPosition(0, 0)
        tortoise.down()

        tortoise.right(2)


def drawing2(tortoise):
    tortoise.down()

    for i in range(500):
        tortoise.setColorHSL(360 * i / 500, 100, 50)
        tortoise.forward(i/2.5)
        tortoise.left(91)


def drawing3(tortoise):
    tortoise.down()

    for k in range(1, 121):
        for n in range(1, 5):
            tortoise.setColorHSL(tortoise.getAngle(), 100, 50)
            tortoise.forward(100)
            tortoise.left(90)
        tortoise.left(3)


def drawing4(tortoise):
    tortoise.down()
    for x in range(360):
        tortoise.setColorHSL(x, 100, 50)
        tortoise.forward(x/2.6)
        tortoise.left(59)


def clear(dom):
    dom.setLayout("SVG", atlastk.createHTML())
    tortoise = Tortoise(dom)
    tortoise.setAutoDraw(autoDraw)
    return tortoise


def call(dom, id):
    tortoise = clear(dom)
    dom.disableElements(ids)
    globals()["drawing" + str(id)](tortoise)
    tortoise.draw()


def acConnect(dom):
    dom.setLayout("", HTML)
    dom.enableElements(ids)
    print(">>>> " + dom.execute("document.getElementById('SVG').getBoundingClientRect().width"))
    print("!!! " + dom.execute("document.getElementById('SVG').width.animVal.value"))
    print(">>> " + dom.execute(svgViewBoxScript))
    print(dom.getProperty("SVG","width"))


def acAll(dom, id):
    call(dom, 0)
    call(dom, 1)
    call(dom, 2)
    call(dom, 3)
    call(dom, 4)
    dom.enableElements(ids)
    globals()["autoDraw"] = 50


def acDraw(dom, id):
    call(dom, id)
    dom.enableElements(ids)


atlastk.launch({"": acConnect, "All": acAll, "Draw": acDraw}, None, HEAD)
