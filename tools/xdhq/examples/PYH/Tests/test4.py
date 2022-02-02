"""
Web storage.
"""

import os, sys

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk

CALLBACKS = {
  "": lambda dom: dom.inner("", open("Test4.html").read()),
  "Set": lambda dom: dom.execute_void(f"localStorage.setItem('{dom.get_value('SetKey')}', '{dom.get_value('SetValue')}');undefined;"),
  "Get": lambda dom: dom.set_value("GetValue",dom.execute_string(f"localStorage.getItem('{dom.get_value('GetKey')}');"))
}


atlastk.launch(CALLBACKS)