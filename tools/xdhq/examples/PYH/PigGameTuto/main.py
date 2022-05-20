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
      atlastk.broadcastAction("Display")


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


def getStatus(player):
  if player != 0:
    return PLAY if player == current else WAIT
  elif available == 0:
    return SPY
  elif available == current:
    return PLAY
  else:
    return WAIT


def fade(dom, element):
  dom.removeClass(element, "fade-in")
  dom.inner(element, "<span></span>")
  dom.flush()
  dom.addClass(element, "fade-in")


def updateMeter(dom, ab, score, adding):
  if adding > 1:
    dom.end(f"ScoreMeter{ab}", METER.format("fade-in dice-meter", adding))
  else:
    dom.inner(f"ScoreMeter{ab}", METER.format("score-meter", score))

  dom.setValue(f"ScoreText{ab}", score)


def getOpponent(player):
  return 2 if player == 1 else 1


def markPlayer(dom, ab):
  dom.disableElement("DisplayMarkerA") if ab == 'B' else dom.enableElement("DisplayMarkerA")


def displayDice(dom, value):
  fade(dom, "dice")

  if value <= 6:
    filename = f"dice-{value}.svg" if value != 0 else "Pig.svg"
    dom.inner("dice", open(filename).read())


def updateMeters(dom, status, adding):
  if status == SPY:
    updateMeter(dom, 'A', scores[1], adding if current == 1 else 0)
    updateMeter(dom, 'B', scores[2], adding if current == 2 else 0)
  else:
    a = current if status == PLAY else getOpponent(current)
    b = getOpponent(a)

    updateMeter(dom, 'A', scores[a], adding if status == PLAY else 0)
    updateMeter(dom, 'B', scores[b], 0 if status == PLAY else adding)


def redrawMeters(dom):
  status = SPY if available == 0 else WAIT

  updateMeters(dom, status, 0)
  updateMeters(dom, status, turn - dice)


def updateMarkers(dom, status):
  if status == WAIT:
    markPlayer(dom, 'B')
  elif status == PLAY:
    markPlayer(dom, 'A')
  else: # SPY
    markPlayer(dom, 'A' if current == 1 else 'B')


def updatePlayButtons(dom, status, winner):
  if status != PLAY or winner != 0:
    dom.disableElements(PLAY_BUTTONS)
  else:
    dom.enableElements(PLAY_BUTTONS)


def displayTurn(dom, element, value):
  fade(dom, element)
  dom.setValue(element, value)


def updateDice(dom, winner):
  if winner != 0 or turn != 0 or dice in [0, 1]:
    displayDice(dom, dice)


def updateTurn(dom, winner):
  if winner == 0:
    displayTurn(dom, "Cumul", turn)
    displayTurn(dom, "Total", scores[current] + turn)


def reportWinner(dom, player, winner):
  ab = 'A' if winner == player or player == 0 and winner == 1 else 'B'

  dom.setValue(f"ScoreMeter{ab}", "<span class='winner'>Winner!</span>")


def updateLayout(dom, player):
  status = getStatus(player)

  if scores[1] >= LIMIT:
    winner = 1
  elif scores[2] >= LIMIT:
    winner = 2
  else:
    winner = 0

  updatePlayButtons(dom, status, winner)
  updateMarkers(dom, status)
  updateDice(dom, winner)
  updateTurn(dom, winner)
  updateMeters(dom, status, dice)

  if winner != 0:
    reportWinner(dom, player, winner)


def display(dom, user):
  if user.pending and available == 0 and user.player == 0:
    dom.disableElement("PlayerView")
    user.pending = False
    redrawMeters(dom)

  updateLayout(dom, user.player)


def acConnect(user, dom):
  dom.inner("", open("Main.html").read())

  if debug():
    dom.removeClass("debug", "removed")

  redrawMeters(dom)

  display(dom, user)


def becomePlayer(user, dom):
  global available

  dom.enableElement("New")
  user.player = current
  user.pending = False

  available = 2 if available == 1 else 0


def acRoll(user, dom):
  global current, dice, turn, scores

  dom.disableElements(PLAY_BUTTONS)

  if user.player == 0:
    becomePlayer(user, dom)
  elif user.player != current:
    return

  dice = random.randint(1, 6)

  if debug():
    debugDice = dom.getValue("debug")
    if debugDice:
      dice = int(debugDice)

  if dice == 1:
    current = getOpponent(current)
    turn = 0
  else:
    turn += dice

    if scores[current] + turn >= LIMIT:
      scores[current] = LIMIT
      turn = 0

  atlastk.broadcastAction("Display")


def acHold(user, dom):
  global scores, turn, dice, current

  dom.disableElements(PLAY_BUTTONS) 

  if user.player == 0:
    becomePlayer(user, dom)
  elif user.player != current:
    return

  scores[user.player] += turn
  current = getOpponent(current)
  dice = turn = 0
  atlastk.broadcastAction("Display")


def acNew():
  init()
  atlastk.broadcastAction("Display")


def acDisplay(user, dom):
  if current == 1 and available == 1:
    dom.inner("", open("Main.html").read())
    dom.enableElement("PlayerView")
    if debug():
      dom.removeClass("debug", "removed")
    user.init()
    dom.flush()
  display(dom, user)


CALLBACKS = {
  "": acConnect,
  "Roll": acRoll,
  "Hold": acHold,
  "New": acNew,
  "Display": acDisplay
}

init()

atlastk.launch(CALLBACKS, User, open("Head.html").read())
