"""
JS console log stuff.
"""

import os, sys

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk

class Switch:
  def __init__(self):
    self.value = False


def ac_switch(switch, dom):
  switch.value = not switch.value
  dom.debug_log(switch.value)
  dom.set_content("Switch", "ON" if switch.value else "OFF")


CALLBACKS = {
  "": lambda switch, dom: dom.inner("", open("Test2.html").read()),
  "Switch": ac_switch,
  "Log": lambda switch, dom: dom.log(dom.get_content("Message"))
}


atlastk.launch(CALLBACKS, Switch)