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

current = 1 # 1 or 2

available = 1 # 0 (no more player available), 1 or 2


class Player:
  def __init__(self):
    self.id = 0


def update_meter(dom, player, current, turn):
  dom.set_attribute(f"player{player}-current", "style", f"width: {current}%;")
  dom.set_attribute(f"player{player}-turn", "style", f"width: {turn}%;")


def disable_buttons(dom):
  dom.disable_elements(["Roll", "Hold"])


def enable_buttons(dom):
  dom.ensable_elements(["Roll", "Hold"])


def ac_connect(player, dom):
  dom.inner("", open("Main.html").read())
  dom.inner("dice", open("dice.svg").read())


def ac_roll(player, dom):
  dom.remove_class("dice", "fade-in")
  dom.flush()
  dom.add_class("dice", "fade-in")
  dom.inner("dice", open(f"dice-{random.randint(1,6)}.svg").read())
  return
  dom.add_class("dice", "opaque")
  dom.remove_class("dice", "fade-in")
  dom.inner("dice", open(f"dice-{random.randint(1,6)}.svg").read())
  dom.add_class("dice", "fade-in")
  dom.remove_class("dice", "opaque")
  

CALLBACKS = {
  "": ac_connect,
  "Roll": ac_roll,
  "Hold": lambda dom :  update_meter(dom, 1, 30, 40),
}

atlastk.launch(CALLBACKS, Player,  open("Head.html").read())
