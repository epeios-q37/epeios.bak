#!/usr/bin/env python3
"""
MIT License

Copyright (c) 2019,2021 Claude SIMON (https://q37.info/s/rmnmqd49)

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

from core import *
import os, sys, time

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk

IMGLayout = True # If at 'True', displays the TXT layout!

def drawBoard(reversi, dom):
  board = atlastk.createHTML("tbody")
  for y, row in enumerate(reversi.board):
    board.push_tag("tr")
    for x, r in enumerate(row):
      board.push_tag("td")
      board.put_attribute("id", str(x) + str(y))
      playable = (r == EMPTY) and (reversi.isAllowed(y, x, reversi.player))
      if playable:
        board.put_attribute("data-xdh-onevent", "Play")
      board.put_attribute(
        "class", {EMPTY: 'none', BLACK: 'black', WHITE: 'white'}[r] + (" playable" if playable else ""))
      board.putValue({EMPTY: ' ', BLACK: 'X', WHITE: 'O'}[r])
      board.pop_tag()
    board.pop_tag()

  dom.inner("board", board)

  dom.set_values({
    "black": reversi.count(BLACK),
    "white": reversi.count(WHITE)
  })

def acConnect(reversi, dom):
  reversi.player = BLACK
  if IMGLayout:
    dom.disable_element("IMG")
  dom.inner("", open("Main.html").read())
  drawBoard(reversi, dom)

def acPlay(reversi, dom, id):
  xy = [int(id[1]), int(id[0])]

  player = reversi.player

  if (reversi.put(xy[0], xy[1], player)):
    drawBoard(reversi, dom)

    xy = reversi.find_best_position(player * -1)
    if xy:
      reversi.put(xy[0], xy[1], player * -1)
      time.sleep(1)

    drawBoard(reversi, dom)

  if (reversi.count(EMPTY) == 0 or
    reversi.count(BLACK) == 0 or
      reversi.count(WHITE) == 0):
    if reversi.count(player) > reversi.count(player * -1):
      dom.alert('You win!')
    elif reversi.count(player) < reversi.count(player * -1):
      dom.alert('You lose!')
    else:
      dom.alert('Egality!')  

def acToggleLayout(reversi, dom):
  global IMGLayout

  if IMGLayout:
    dom.enable_element("IMG")
  else:
    dom.disable_element("IMG")

  IMGLayout = not IMGLayout

def acNew(reversi, dom):
  reversi.reset()
  drawBoard(reversi, dom)


callbacks = {
  "": acConnect,
  "Play": acPlay,
  "ToggleLayout": acToggleLayout,
  "New": acNew,
}

atlastk.launch(callbacks, lambda : Reversi(2), open("Head.html").read())