from tortoise import *

HTML = """
<div style="display: table; margin: auto;">
 <svg id="SVG" width="300" height="300" />
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
    dom.setLayout("SVG", Atlas.createHTML())
    tortoise = Tortoise(dom)
    tortoise.setAutoDraw(autoDraw)
    return tortoise


def call(dom, id):
    tortoise = clear(dom)
    dom.disableElements(ids)
    globals()["drawing" + str(id)](tortoise)
    tortoise.draw()


def acConnect(dummy, dom):
    dom.setLayout("", HTML)
    dom.enableElements(ids)


def acAll(dummy, dom, id):
    call(dom, 0)
    call(dom, 1)
    call(dom, 2)
    call(dom, 3)
    call(dom, 4)
    dom.enableElements(ids)
    globals()["autoDraw"] = 50


def acDraw(dummy, dom, id):
    call(dom, id)
    dom.enableElements(ids)


Atlas.launch({"": acConnect, "All": acAll, "Draw": acDraw}, None, HEAD)
