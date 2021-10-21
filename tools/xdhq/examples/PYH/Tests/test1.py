"""
To test if a session lopps, if other sessions are blocked.
Answer: No (handled by the JS scripts)
"""

import atlastk, datetime, time

BODY = """
<fieldset >
  <button xdh:onevent="Control">Control</button>
  <button xdh:onevent="UBlock">Uninterrupted blocking</button>
  <button xdh:onevent="IBlock">Interrupted blocking</button>
  <hr/>
  <fieldset>
    <output id="Output"/>
  </fieldset>
</fieldset>
"""

def ac_ublock(dom):
  dom.begin("Output",f"<div>Uninterrupted blocking: {datetime.datetime.now()}</div>")
  while True:
    pass

def ac_iblock(dom):
  while True:
    dom.begin("Output",f"<div>Interrupted blocking: {datetime.datetime.now()}</div>")
    time.sleep(3);

CALLBACKS = {
  "": lambda dom: dom.inner("", BODY),
  "Control": lambda dom: dom.begin("Output",f"<div>Control: {datetime.datetime.now()}</div>"),
  "UBlock": ac_ublock,
  "IBlock": ac_iblock
}

atlastk.launch(CALLBACKS)