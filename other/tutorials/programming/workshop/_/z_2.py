# coding: utf-8

import math
import workshop._._ as _

_dir = "z_2"

_.store('posx',100)
_.store('posy',100)
_.store('angle', 0)
_.store('stack',[])

def _push(path):
  _.recall('stack').append(path)

_push("M 100 100")


def forward(distance):
  angle = _.recall('angle')
  posx = _.recall('posx') + distance * math.sin(angle)
  posy = _.recall('posy') - distance * math.cos(angle)

  _push("L {} {}".format(posx, posy))

  _.store('posx', posx)
  _.store('posy', posy)

def right(angle):
  _.store('angle',math.radians((math.degrees(_.recall('angle')) + angle ) % 360))

def left(angle):
  right(360 - angle)

def _mergePaths():
  mergedPaths = ""

  for path in _.recall('stack'):
    mergedPaths += " " + path

  return mergedPaths

def _acConnect(d,dom,id):
  dom.setLayout("",_.readBody(_dir))

  html = _.Atlas.createHTML()
  html.pushTag("path")
  html.putAttribute("stroke", "blue")
  html.putAttribute("stroke-width", "2")
  html.putAttribute("fill", "none")
  html.putAttribute('d', _mergePaths())

  dom.setLayout("SVG", html)

def main(callback,title):
  _.main(_dir, callback, {
      "" : _acConnect,
    }, title )
