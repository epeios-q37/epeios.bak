from tortoise import *

HTML = """
<div style="display: table; margin: auto;">
  <svg id="SVG" width="300" height="300" />
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

#

def koch(tortoise,l):
    if l < 3:
        tortoise.setColorHSL(tortoise.getAngle(),100,50)
        tortoise.forward(l*2)
    else:
        koch(tortoise,l/3.0)
        tortoise.left(60)
        koch(tortoise, l/3.0)
        tortoise.right(120)
        koch(tortoise,l/3.0)
        tortoise.left(60)
        koch(tortoise, l/3.0)


def pattern1(tortoise):
    tortoise.right(90)
    tortoise.forward(150)
    tortoise.left(180)
    tortoise.down()
    for _ in range(3):
        koch(tortoise, 100)
        tortoise.right(120)

#

def pattern2(tortoise):
    tortoise.down()
    for x in range(360):
        tortoise.setColorHSL(x, 100, 50)
        tortoise.forward(x/2.6)
        tortoise.left(59)


#

def pattern3(tortoise):
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


#

def pattern4(tortoise):
    tortoise.down()

    for i in range(500):
        tortoise.setColorHSL( 360 * i / 500, 100, 50)
        tortoise.forward(i/2.5)
        tortoise.left(91)

#


def pattern5(tortoise):
    tortoise.down()
    tortoise.setAutoDraw(4)

    for k in range(1,121):
        for n in range(1,5):
            tortoise.setColorHSL(tortoise.getAngle(), 100, 50)
            tortoise.forward(100)
            tortoise.left(90)
        tortoise.left(3)


#

def clear(dom):
    dom.setLayout("", HTML)
    tortoise = Tortoise(dom)
    tortoise.setAutoDraw(100)
    return tortoise


def acConnect(dummy,dom):
    while True:
        tortoise = clear(dom)
        pattern1(tortoise)
        tortoise.draw()
        tortoise = clear(dom)
        pattern2(tortoise)
        tortoise.draw()
        tortoise = clear(dom)
        pattern3(tortoise)
        tortoise.draw()
        tortoise = clear(dom)
        pattern4(tortoise)
        tortoise.draw()
        tortoise = clear(dom)
        pattern5(tortoise)
        tortoise.draw()



Atlas.launch({"": acConnect}, None, HEAD)
