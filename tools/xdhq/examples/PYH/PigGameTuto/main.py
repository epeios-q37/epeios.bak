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

LIMIT = 100

current = 1 # 1 or 2

available = 1 # 0 (no more player available), 1 or 2

scores = {
  1: 0,
  2: 0
}


turn = 0
dice = 0

class Player:
  def __init__(self):
    self.id = 0


SPY = 0
PLAY = 1
WAIT = 2


def get_status(id):
  if id == 0:
    if available == 0:
      return SPY
    elif available == current:
      return PLAY
    else:
      return WAIT
  elif id == current:
    return PLAY
  else:
    return WAIT


def update_meter(dom, player, actual, turn):
  if current + turn > LIMIT:
    turn = LIMIT - current

  dom.set_attribute(f"player{player}-current", "style", f"width: {actual}%;")
  dom.set_attribute(f"player{player}-turn", "style", f"width: {turn}%;")


def disable_buttons(dom):
  dom.disable_elements(["Roll", "Hold"])


def enable_buttons(dom):
  dom.enable_elements(["Roll", "Hold"])


def get_opponent(player):
  return 2 if player == 1 else 1


def point_player(dom, player):
  dom.remove_class(f"arrow{player}", "hide")
  dom.add_class(f"arrow{get_opponent(player)}", "hide")  


def update_layout(dom, status):
  if status == WAIT:
    disable_buttons(dom)
    point_player(dom, 2)
  elif status == PLAY:
    enable_buttons(dom)
    point_player(dom, 1)
  else:
    disable_buttons(dom)
    point_player(dom, current)


def display_dice(dom, value):
  dom.remove_class("dice", "fade-in-dice")
  dom.flush()
  dom.add_class("dice", "fade-in-dice")

  filename = f"dice-{value}.svg" if value != 0 else "Pig.svg"
  dom.inner("dice", open(filename).read())


def display(player, dom):
  if available == 0 and player.id == 0:
    dom.set_contents({
      "Label1": "Player 1",
      "Label2": "Player 2"
    })

  status = get_status(player.id)
  update_layout(dom, status)
  if status == SPY:
    update_meter(dom, 1, scores[1], turn if current == 1 else 0)
    update_meter(dom, 2, scores[2], turn if current == 2 else 0)
  else:
    id = current

    if status == WAIT:
      id = get_opponent(id)

    opponent = get_opponent(player.id)
    me = id == current

    update_meter(dom, 1, scores[id], turn if me else 0)
    update_meter(dom, 2, scores[opponent], 0 if me else turn)

  display_dice(dom, dice)

  dom.remove_class("Turn", "fade-in-score")
  dom.flush()
  dom.add_class("Turn", "fade-in-score")
  dom.set_content("Turn", turn)


def ac_connect(player, dom):
  dom.inner("", open("Main.html").read())
  display(player, dom)



def ac_roll(player, dom):
  global available, dice, turn

  if player.id == 0:
    player.id = current

    if available == 1:
      available = 2
    elif available == 2:
      available = 0

  dice = random.randint(1, 6)
  turn += dice


  atlastk.broadcast_action("Display")


def ac_hold(player, dom):
  global scores, turn, current
  scores[player.id] += turn
  current = get_opponent(current)
  turn = 0
  atlastk.broadcast_action("Display")



def ac_display(player, dom):
  display(player, dom)


CALLBACKS = {
  "": ac_connect,
  "Roll": ac_roll,
  "Hold": ac_hold,
  "Display": ac_display
}

atlastk.launch(CALLBACKS, Player, open("Head.html").read())
