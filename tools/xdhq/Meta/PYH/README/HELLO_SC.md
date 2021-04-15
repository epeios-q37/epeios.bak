```python
import atlastk

BODY = """
<fieldset>
 <input id="Input" data-xdh-onevent="Submit" value="World"/>
 <button data-xdh-onevent="Submit">Hello</button>
 <hr/>
 <fieldset>
  <output id="Output">Greetings displayed here!</output>
 </fieldset>
</fieldset>
"""

def ac_connect(dom):
  dom.inner("", BODY)
  dom.focus("Input")

def ac_submit(dom):
  name = dom.get_value("Input")
  dom.set_value("Output", f"Hello, {name}]!")
  dom.set_value("Input", "")
  dom.focus("Input")

callbacks = {
  "": ac_connect,  # The action label for a new connection is an empty string.
  "Submit": ac_submit
}

atlastk.launch(callbacks)
```
