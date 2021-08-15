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

SPY = 0
PLAY = 1
WAIT = 2

METER = '<span class="{}" style="width: {}%;"></span>'

PLAY_BUTTONS = ["Roll", "Hold"]


class User:
  def init(self):
    self.player = 0
    self.pending = True

  def __init__(self):
    self.init()

  def __del__(self):
    if self.player != 0:
      init()
      atlastk.broadcast_action("Display")


def debug():
  try:
    return DEBUG
  except NameError:
    return False


def init():
  global current, available, scores, turn, dice

  current = 1 # 1 or 2

  available = 1 # 0 (no player available), 1 or 2

  scores = {
    1: 0,
    2: 0
  }

  turn = dice = 0


def get_status(player):
  if player != 0:
    return PLAY if player == current else WAIT
  elif available == 0:
    return SPY
  elif available == current:
    return PLAY
  else:
    return WAIT


def fade(dom, element):
  dom.remove_class(element, "fade-in")
  dom.inner(element, "<span></span>")
  dom.flush()
  dom.add_class(element, "fade-in")


def update_meter(dom, ab, score, adding):
  if adding > 1:
    dom.end(f"ScoreMeter{ab}", METER.format("fade-in dice-meter", adding))
  else:
    dom.inner(f"ScoreMeter{ab}", METER.format("score-meter", score))

  dom.set_content(f"ScoreText{ab}", score)


def get_opponent(player):
  return 2 if player == 1 else 1


def mark_player(dom, ab):
  dom.disable_element("DisplayMarkerA") if ab == 'B' else dom.enable_element("DisplayMarkerA")


def display_dice(dom, value):
  fade(dom, "dice")

  if value <= 6:
    filename = f"dice-{value}.svg" if value != 0 else "Pig.svg"
    dom.inner("dice", open(filename).read())


def update_meters(dom, status, adding):
  if status == SPY:
    update_meter(dom, 'A', scores[1], adding if current == 1 else 0)
    update_meter(dom, 'B', scores[2], adding if current == 2 else 0)
  else:
    a = current if status == PLAY else get_opponent(current)
    b = get_opponent(a)

    update_meter(dom, 'A', scores[a], adding if status == PLAY else 0)
    update_meter(dom, 'B', scores[b], 0 if status == PLAY else adding)


def redraw_meters(dom):
  status = SPY if available == 0 else WAIT

  update_meters(dom, status, 0)
  update_meters(dom, status, turn - dice)


def update_markers(dom, status):
  if status == WAIT:
    mark_player(dom, 'B')
  elif status == PLAY:
    mark_player(dom, 'A')
  else: # SPY
    mark_player(dom, 'A' if current == 1 else 'B')


def update_play_buttons(dom, status, winner):
  if status != PLAY or winner != 0:
    dom.disable_elements(PLAY_BUTTONS)
  else:
    dom.enable_elements(PLAY_BUTTONS)


def display_turn(dom, element, value):
  fade(dom, element)
  dom.set_content(element, value)


def update_dice(dom, winner):
  if winner != 0 or turn != 0 or dice in [0, 1]:
    display_dice(dom, dice)


def update_turn(dom, winner):
  if winner == 0:
    display_turn(dom, "Cumul", turn)
    display_turn(dom, "Total", scores[current] + turn)


def report_winner(dom, player, winner):
  ab = 'A' if winner == player or player == 0 and winner == 1 else 'B'

  dom.set_content(f"ScoreMeter{ab}", "<span class='winner'>Winner!</span>")


def update_layout(dom, player):
  status = get_status(player)

  if scores[1] >= LIMIT:
    winner = 1
  elif scores[2] >= LIMIT:
    winner = 2
  else:
    winner = 0

  update_play_buttons(dom, status, winner)
  update_markers(dom, status)
  update_dice(dom, winner)
  update_turn(dom, winner)
  update_meters(dom, status, dice)

  if winner != 0:
    report_winner(dom, player, winner)


def display(dom, user):
  if user.pending and available == 0 and user.player == 0:
    dom.disable_element("PlayerView")
    user.pending = False
    redraw_meters(dom)

  update_layout(dom, user.player)


def ac_connect(user, dom):
  dom.inner("", open("Main.html").read())

  if debug():
    dom.remove_class("debug", "removed")

  redraw_meters(dom)

  display(dom, user)


def become_player(user, dom):
  global available

  dom.enable_element("New")
  user.player = current
  user.pending = False

  available = 2 if available == 1 else 0


def ac_roll(user, dom):
  global current, dice, turn, scores

  dom.disable_elements(PLAY_BUTTONS)

  if user.player == 0:
    become_player(user, dom)
  elif user.player != current:
    return

  dice = random.randint(1, 6)

  if debug():
    debug_dice = dom.get_content("debug")
    if debug_dice:
      dice = int(debug_dice)

  if dice == 1:
    current = get_opponent(current)
    turn = 0
  else:
    turn += dice

    if scores[current] + turn >= LIMIT:
      scores[current] = LIMIT
      turn = 0

  atlastk.broadcast_action("Display")


def ac_hold(user, dom):
  global scores, turn, dice, current

  dom.disable_elements(PLAY_BUTTONS) 

#  dom.raw_send("%Quit")

  if user.player == 0:
    become_player(user, dom)
  elif user.player != current:
    return

  scores[user.player] += turn
  current = get_opponent(current)
  dice = turn = 0
  atlastk.broadcast_action("Display")


def ac_new():
  init()
  atlastk.broadcast_action("Display")


def ac_display(user, dom):
  if current == 1 and available == 1:
    dom.inner("", open("Main.html").read())
    dom.enable_element("PlayerView")
    if debug():
      dom.remove_class("debug", "removed")
    user.init()
    dom.flush()
  display(dom, user)


CALLBACKS = {
  "": ac_connect,
  "Roll": ac_roll,
  "Hold": ac_hold,
  "New": ac_new,
  "Display": ac_display
}

init()

atlastk.launch(CALLBACKS, User, open("Head.html").read())
