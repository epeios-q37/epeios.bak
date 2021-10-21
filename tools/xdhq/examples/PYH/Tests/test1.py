"""
To test if a session lopps, if other sessions are blocked.
Answer: No (handled by the JS scripts)
"""

import atlastk, datetime

BODY = """
<fieldset >
  <button xdh:onevent="Control">Control</button>
  <button xdh:onevent="Block">Block</button>
  <hr/>
  <fieldset>
    <output id="Output"/>
  </fieldset>
</fieldset>
"""

def ac_block(dom):
  while True:
    pass

CALLBACKS = {
  "": lambda dom: dom.inner("", BODY),
  "Control": lambda dom: dom.begin("Output",f"<div>Control: {datetime.datetime.now()}</div>"),
  "Block": ac_block
}

atlastk.launch(CALLBACKS)