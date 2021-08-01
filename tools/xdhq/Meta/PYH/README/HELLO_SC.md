```python
import atlastk
 
BODY = """
<fieldset>
 <input id="Input" xdh:onevent="Submit" value="World"/>
 <button xdh:onevent="Submit">Hello</button>
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
  dom.begin("Output", f"<div>Hello, {name}!</div>")
  dom.set_value("Input", "")
  dom.focus("Input")
 
CALLBACKS = {
  "": ac_connect,
  "Submit": ac_submit
}
 
atlastk.launch(CALLBACKS)
```