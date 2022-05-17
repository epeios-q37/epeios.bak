"""
Web storage.
"""

import os, sys

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk

CALLBACKS = {
  "": lambda dom: dom.inner("", open("Test4.html").read()),
  "Set": lambda dom: dom.executeVoid(f"localStorage.setItem('{dom.getValue('SetKey')}', '{dom.getValue('SetValue')}');undefined;"),
  "Get": lambda dom: dom.setValue("GetValue",dom.executeString(f"localStorage.getItem('{dom.getValue('GetKey')}');"))
}


atlastk.launch(CALLBACKS)