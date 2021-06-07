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

import os, sys, itertools

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.extend(["../../atlastk", "."])

import atlastk

EMPTY = 0
BLACK = -1
WHITE = 1

TOKEN = {
  EMPTY: ' ',
  BLACK: 'X',
  WHITE: 'O'
}

board = None
turn = None

def has_my_piece(bw, x, y, delta_x, delta_y):
  "There is my piece in the direction of (delta_x, delta_y) from (x, y)."
  assert bw in (BLACK, WHITE)
  assert delta_x in (-1, 0, 1)
  assert delta_y in (-1, 0, 1)
  x += delta_x
  y += delta_y

  if x < 0 or x > 7 or y < 0 or y > 7 or board[x][y] == EMPTY:
    return False
  if board[x][y] == bw:
    return True
  return has_my_piece(bw, x, y, delta_x, delta_y)

def reversible_directions(bw, x, y):
  "Can put piece on (x, y) ? Return list of reversible direction tuple"
  assert bw in (BLACK, WHITE)

  directions = []
  if board[x][y] != EMPTY:
    return directions

  for d in itertools.product([-1, 1, 0], [-1, 1, 0]):
    if d == (0, 0):
      continue
    nx = x + d[0]
    ny = y + d[1]
    if nx < 0 or nx > 7 or ny < 0 or ny > 7 or board[nx][ny] != bw * -1:
      continue
    if has_my_piece(bw, nx, ny, d[0], d[1]):
      directions.append(d)
  return directions

def is_allowed(x, y, bw):
  return len(reversible_directions(bw, x, y)) != 0


def init_board():
  global board

  board = []

  for _ in range(8):
    board.append([EMPTY] * 8)

  board[3][3] = board[4][4] = BLACK
  board[4][3] = board[3][4] = WHITE


class Reversi:
  def __init__(self):
    self.bw = EMPTY

  def init(self, bw):
    self.bw = bw

def draw_board(reversi, dom, playability = True):
  tokens = {}
  toAdd = {}
  toRemove = {}

  for y, row in enumerate(board):
    for x, r in enumerate(row):
      tokens[f'{x}{y}'] = TOKEN[r]
      playable = playability and (r == EMPTY) and (is_allowed(y, x, reversi.bw if reversi.bw != EMPTY else BLACK))

      if ( playable ):
        toAdd[f'{x}{y}'] = "playable"
      else:
        toRemove[f'{x}{y}'] = "playable"

  dom.set_values(tokens)
  dom.addClasses(toAdd)
  dom.removeClasses(toRemove)


def ac_connect(reversi, dom, id):
  if ( board == None ):
    init_board()
  dom.inner("", open("TutoMain.html").read())
  draw_board(reversi, dom)


CALLBACKS = {
  "": ac_connect,
}

atlastk.launch(CALLBACKS,  Reversi, open("TutoHead.html").read())