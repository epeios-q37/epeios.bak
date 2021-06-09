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
available = None
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

def reverse_piece(bw, x, y, delta_x, delta_y):
  global board
  "Reverse pieces in the direction of (delta_x, delta_y) from (x, y) until bw."
  assert bw in (BLACK, WHITE)

  x += delta_x
  y += delta_y
  assert board[x][y] in (BLACK, WHITE)

  if board[x][y] == bw:
    return

  board[x][y] = bw
  return reverse_piece(bw, x, y, delta_x, delta_y)  

def is_allowed(x, y, bw):
  if bw == EMPTY:
    return False
  else:
    return len(reversible_directions(bw, x, y)) != 0


def init_board():
  global board, available, turn

  board = []

  for _ in range(8):
    board.append([EMPTY] * 8)

  board[3][3] = board[4][4] = BLACK
  board[4][3] = board[3][4] = WHITE

  available = True
  turn = BLACK


class Reversi:
  def __init__(self):
    self.bw = None

  def init(self, bw):
    self.bw = bw

def set_status(dom, status, color = "black"):
  dom.inner("Status",f'<span style="color: {color}">{status}</span>')    

def draw_board(reversi, dom):
  tokens = {}
  toAdd = {}
  toRemove = {}

  bw = reversi.bw

  if bw == None:
    bw = turn

  for x, row in enumerate(board):
    for y, r in enumerate(row):
      tokens[f'{x}{y}'] = TOKEN[r]
      playable = (r == EMPTY) and bw == turn and is_allowed(x, y, bw)

      if ( playable ):
        toAdd[f'{x}{y}'] = "playable"
      else:
        toRemove[f'{x}{y}'] = "playable"

  dom.set_values(tokens)
  dom.addClasses(toAdd)
  dom.removeClasses(toRemove)


def count(bwe):
  "Count pieces or empty spaces in the board"
  assert bwe in (BLACK, WHITE, EMPTY)
  n = 0
  for i in range(8):
    for j in range(8):
      if board[i][j] == bwe:
        n += 1
  return n


def Refresh(dom, reversi):
  if reversi.bw == None:
    if not available:
      reversi.bw = EMPTY
  draw_board(reversi, dom)

  dom.set_values({
    "black": count(BLACK),
    "white": count(WHITE)
  })

#  return
  if reversi.bw == EMPTY:
    set_status(dom, f"'{TOKEN[turn]}' (you cannot play)")
  elif reversi.bw == None:
    if turn == BLACK:
      set_status(dom, "Play ('X') or wait opponent's move!", "blue")
    else:
      set_status(dom, "First player ('X') played!", "blue")
  elif reversi.bw == turn:
    set_status(dom, f"Your turn ('{TOKEN[turn]}')!", "green")
  else:
    set_status(dom, "Wait for opponent's move!", 'red"')


def ac_connect(reversi, dom, id):
  if ( board == None ):
    init_board()
  if not available:
    reversi.bw = EMPTY
  dom.inner("", open("Main.html").read())
  Refresh(dom, reversi)


def ac_play(reversi, dom, id):
  global board, turn, available

  x, y = int(id[0]), int(id[1])

  bw = reversi.bw

  if bw == None:
    bw = reversi.bw = turn
    available = turn == BLACK

  if ( bw != EMPTY ) and bw == turn:
    if is_allowed(x, y, bw):
      for delta in reversible_directions(bw, x, y):
        reverse_piece(bw, x, y, delta[0], delta[1])
      board[x][y] = bw

      turn = turn * -1

    atlastk.broadcast_action("Refresh")

def ac_refresh(reversi, dom):
  Refresh(dom, reversi)


CALLBACKS = {
  "": ac_connect,
  "Play": ac_play,
  "Refresh": ac_refresh,
  "New": 
}

atlastk.launch(CALLBACKS,  Reversi, open("Head.html").read())