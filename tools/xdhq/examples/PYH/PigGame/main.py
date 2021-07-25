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

import atlastk, random

DEBUG = True  # Uncomment for debug mode.

LIMIT = 100

class Game:
  def __init__(self):
    self.current = 1 # 1 or 2

    self.available = 1 # 0 (no more player available), 1 or 2

    self.scores = {
      1: 0,
      2: 0
    }

    self.turn = 0  
    self.dice = 0

lock = threading.Lock()
games = {}


def get_games(token):
  global lock

  with lock:
    return games[token]


def fade(dom, element):
  dom.remove_class(element, "fade-in")
  dom.flush()
  dom.add_class(element, "fade-in")


class User:
  def __init__(self):
    self.player = 0
    self.game = None 
    self.token = None # if None, opponent is the computer.

  def get_game(self):
    if self.token:
      return get_games(self.token)
    else:
      return self.game


def fade(dom, element):
  dom.remove_class(element, "fade-in")
  dom.flush()
  dom.add_class(element, "fade-in")


def display_dice(dom, value):
  fade(dom, "dice")

  if value <= 6:
    filename = f"dice-{value}.svg" if value != 0 else "Pig.svg"
    dom.inner("dice", open(filename).read())


def update_dice(dom, game, winner):
  if winner != 0 or game.turn != 0 or game.dice == 0:
    display_dice(dom, dice)


def update_layout(dom, game):
  update_dice(dom)


def ac_connect(user, dom):
  dom.inner("", open("Main.html").read())
  display(dom)


CALLBACKS = {
  "": ac_connect
}

atlastk.launch(CALLBACKS, User, open("Head.html").read())