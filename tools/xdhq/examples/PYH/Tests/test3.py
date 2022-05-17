"""
Programmatically checking radio-buttons.
"""

import os, sys

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk

CALLBACKS = {
  "": lambda dom: dom.inner("", open("Test3.html").read()),
  "rdSelect": lambda dom, id: dom.setValue("rdCheck", dom.getValue(id)),
  "rdSubmit": lambda dom, id: dom.alert(f'Selected method: "{dom.getValue("rdContact")}"' ),
  "rdCheck": lambda dom, id: dom.setValue("rdContact", dom.getValue(id))
}

atlastk.launch(CALLBACKS)