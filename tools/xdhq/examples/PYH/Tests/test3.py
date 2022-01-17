"""
Programmatically checking radio-buttons.
"""

import os, sys

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk

CALLBACKS = {
  "": lambda dom: dom.inner("", open("Test3.html").read()),
  "rdSelect": lambda dom, id: dom.set_value("rdOutput", dom.get_value(id)),
  "rdSubmit": lambda dom, id: dom.alert(f'Selected method: "{dom.get_value(id)}"' ),
  "rdCheck": lambda dom, id: dom.raw_send(f"document.querySelector('input[name=\"rdContact\"][value=\"{dom.get_value(id)}\"]').checked=true;"),
}

atlastk.launch(CALLBACKS)