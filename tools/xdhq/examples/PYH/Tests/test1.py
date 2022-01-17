"""
Session loops.
"""

import os, sys

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk, datetime, time

SLEEP_DELAY = 3

def content(label):
  return f"<div>'{label}': {datetime.datetime.now().time()}</div>"

def ac_uloop(dom, id):
  dom.begin("Output",content("while True:"))
  # Corresponding instance can never be released, even if closed in the browser.
  while True:
    pass

def ac_iloop_b(dom, id):
  while True:
    dom.begin("Output",content("Begin(…)"))
    time.sleep(SLEEP_DELAY)

def ac_iloop_c(dom, id):
  while True:
    dom.set_content("Output",content("SetContent(…)"))
    time.sleep(SLEEP_DELAY)

CALLBACKS = {
  "": lambda dom: dom.inner("", open("Test1.html").read()),
  "Control": lambda dom, id: dom.begin("Output", content("Control")),
  "ULoop": ac_uloop,
  "ILoopB": ac_iloop_b,
  "ILoopC": ac_iloop_c
}

atlastk.launch(CALLBACKS)