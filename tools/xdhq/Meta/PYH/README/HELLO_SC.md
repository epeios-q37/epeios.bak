```python
import atlastk as Atlas

body = """
<div style="display: table; margin: 50px auto auto auto;">
 <fieldset>
  <input id="input" maxlength="20" placeholder="Enter a name here" type="text"
         data-xdh-onevent="Submit" value="World"/>
  <div style="display: flex; justify-content: space-around; margin: 5px auto auto auto;">
   <button data-xdh-onevent="Submit">Submit</button>
   <button data-xdh-onevent="Clear">Clear</button>
  </div>
 </fieldset>
</div>
"""

def ac_connect(dom):
  dom.set_layout("", body)
  dom.focus("input")

def ac_submit(dom):
  dom.alert("Hello, " + dom.get_content("input") + "!")
  dom.focus("input")

def ac_clear(dom):
  if ( dom.confirm("Are you sure?") ):
    dom.set_content("input", "")
  dom.focus("input")

callbacks = {
  "": ac_connect,  # The action label for a new connection is an empty string.
  "Submit": ac_submit,
  "Clear": ac_clear,
}

Atlas.launch(callbacks)
```
