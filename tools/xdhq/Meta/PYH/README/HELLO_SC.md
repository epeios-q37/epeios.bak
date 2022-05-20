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
 
def acConnect(dom):
  dom.inner("", BODY)
  dom.focus("Input")
 
def acSubmit(dom):
  name = dom.getValue("Input")
  dom.begin("Output", f"<div>Hello, {name}!</div>")
  dom.setValue("Input", "")
  dom.focus("Input")
 
CALLBACKS = {
  "": acConnect,
  "Submit": acSubmit
}
 
atlastk.launch(CALLBACKS)
```