#!/usr/bin/env python3
"""
MIT License

Copyright (c) 2021 Claude SIMON (https://q37.info/s/rmnmqd49)

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

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.extend(["../../atlastk", "."])

from board import *
import atlastk

board = None
available = None
turn = None


class Player:
  def __init__(self):
    self.bw = None

  def init(self, bw):
    self.bw = bw  

def init():
  global board, available, turn
  board = new_board()
  available = True
  turn = None

def set_status(dom, status, color = "black"):
  dom.inner("Status", f'<span style="color: {color}">{status}</span>')    

def draw_board(player, dom):
  tokens = {}
  toAdd = {}
  toRemove = {}

  bw = player.bw or turn or BLACK

  for x, row in enumerate(board):
    for y, r in enumerate(row):
      tokens[f'{x}{y}'] = TOKEN[r]
      playable = (r == EMPTY) and bw == ( turn or BLACK ) and is_allowed(board, x, y, bw)

      if ( playable ):
        toAdd[f'{x}{y}'] = "playable"
      else:
        toRemove[f'{x}{y}'] = "playable"

  dom.set_values(tokens)
  dom.addClasses(toAdd)
  dom.removeClasses(toRemove)


def debug(dom, player):
  html = f'''
<div>available: {available}</div>
<div>turn: {turn}</div>
<div>player.bw: {player.bw}</div>
  '''

  dom.inner("Debug", html)

def Refresh(dom, player):
  if player.bw == None:
    if not available:
      player.bw = EMPTY
      dom.disable_element("Submit")
    else:
      dom.enable_element("Submit")
  draw_board(player, dom)

  dom.set_values({
    "black": count(board, BLACK),
    "white": count(board, WHITE)
  })
  debug(dom, player)

  if player.bw == EMPTY:
    set_status(dom, f"'{TOKEN[turn]}''s turn (you cannot play)")
  elif player.bw == None:
    if turn == WHITE:
      set_status(dom, "First player ('X') played!", "blue")
    else:
      set_status(dom, "Play ('X') or wait opponent's move!", "blue")
  elif player.bw == turn:
    set_status(dom, f"Your turn ('{TOKEN[turn]}')!", "green")
  else:
    set_status(dom, "Wait for opponent's move!", 'red"')


def ac_connect(player, dom, id):
  if ( board == None ):
    init()
  dom.inner("", open("Main.html").read())
  if not available:
    player.bw = EMPTY
    dom.disable_element("Submit")
  Refresh(dom, player)


def ac_play(player, dom, id):
  global board, turn, available

  x, y = int(id[0]), int(id[1])

  bw = player.bw

  if not is_allowed(board, x, y, bw or turn or BLACK):
    return

  if turn == None:
    turn = BLACK

  if bw == None:
    bw = player.bw = turn
    available = turn == BLACK

  if ( bw != EMPTY ) and bw == turn:
    for delta in reversible_directions(board, bw, x, y):
      reverse_piece(
        board, bw, x, y, delta[0], delta[1])
    board[x][y] = bw

    turn = turn * -1

  atlastk.broadcast_action("Refresh")

def ac_refresh(player, dom):
  if turn == None:
    player.__init__()
  Refresh(dom, player)

def ac_new(dom):
  init()
  atlastk.broadcast_action("Refresh")


CALLBACKS = {
  "": ac_connect,
  "Play": ac_play,
  "Refresh": ac_refresh,
  "New": ac_new
}

atlastk.launch(CALLBACKS,  Player, open("Head.html").read())
