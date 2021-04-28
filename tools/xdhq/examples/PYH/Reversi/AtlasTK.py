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

import core
import os, sys, time

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk

COMPUTER = 0
HUMAN = 1
DELAY = 1 # Delay in seconds before computer move.

layoutFlag = True # If at 'True', displays the TXT layout!

class Reversi:
  def __init__(self):
    self.board = None
    self.level = 0
    self.bw = core.EMPTY
    self.opponent = None

  def init(self, level, bw, opponent):
    self.board = core.Board()
    self.level = level
    self.bw = bw
    self.opponent = opponent

def drawBoard(reversi, dom):
  board = atlastk.createHTML("tbody")
  for y, row in enumerate(reversi.board.array()):
    board.push_tag("tr")
    for x, r in enumerate(row):
      board.push_tag("td")
      board.put_attribute("id", str(x) + str(y))
      playable = (r == core.EMPTY) and (reversi.board.isAllowed(y, x, reversi.bw if reversi.bw != core.EMPTY else core.BLACK))
      if playable:
        board.put_attribute("data-xdh-onevent", "Play")
      board.put_attribute(
        "class", {core.EMPTY: 'none', core.BLACK: 'black', core.WHITE: 'white'}[r] + (" playable" if playable else ""))
      board.putValue({core.EMPTY: ' ', core.BLACK: 'X', core.WHITE: 'O'}[r])
      board.pop_tag()
    board.pop_tag()

  dom.inner("board", board)

  dom.set_values({
    "black": reversi.board.count(core.BLACK),
    "white": reversi.board.count(core.WHITE)
  })

def acConnect(reversi, dom):
  if layoutFlag:
    dom.disable_element("IMG")
  dom.inner("", open("Main.html").read())
  reversi.init(int(dom.get_value("level")), core.BLACK, COMPUTER)
  drawBoard(reversi, dom)

def computerMove(reversi, dom):
  bw = reversi.bw
  board = reversi.board
  xy = board.find_best_position(bw * -1, reversi.level)
  if xy:
    board.put(xy[0], xy[1], bw * -1)
    time.sleep(DELAY)

  drawBoard(reversi, dom)

def acPlay(reversi, dom, id):
  xy = [int(id[1]), int(id[0])]

  bw = reversi.bw
  board = reversi.board

  if (board.put(xy[0], xy[1], bw)):
    drawBoard(reversi, dom)

  computerMove(reversi, dom)

  if (board.count(core.EMPTY) == 0 or
    board.count(core.BLACK) == 0 or
      board.count(core.WHITE) == 0):
    if board.count(bw) > board.count(bw * -1):
      dom.alert('You win!')
    elif board.count(bw) < board.count(bw * -1):
      dom.alert('You lose!')
    else:
      dom.alert('Egality!')  

def acToggleLayout(reversi, dom):
  global layoutFlag

  if layoutFlag:
    dom.enable_element("IMG")
  else:
    dom.disable_element("IMG")

  layoutFlag = not layoutFlag

def acNew(reversi, dom):
  players = dom.get_value("players")
  bw = core.EMPTY
  if players == "HC":
    bw = core.BLACK
  elif players == "CH":
    bw = core.WHITE
  reversi.init(int(dom.get_value("level")), bw, HUMAN if players == "HH" else COMPUTER)

  drawBoard(reversi, dom)

  if bw == core.WHITE:
    time.sleep(DELAY)
    computerMove(reversi,dom)

callbacks = {
  "": acConnect,
  "Play": acPlay,
  "ToggleLayout": acToggleLayout,
  "New": acNew,
  "SelectMode": lambda reversi, dom, id: dom.set_attribute("level", "style", f"visibility: {'hidden' if dom.get_value(id) == 'HH' else 'visible'};")
}

atlastk.launch(callbacks, Reversi, open("Head.html").read())