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

def acConnect(dom):
  dom.setLayout("", body)
  dom.focus("input")

def acSubmit(dom):
  dom.alert("Hello, " + dom.getContent("input") + "!")
  dom.focus("input")

def acClear(dom):
  if ( dom.confirm("Are you sure?") ):
    dom.setContent("input", "")
  dom.focus("input")

callbacks = {
  "": acConnect,  # The action label for a new connection is an empty string.
  "Submit": acSubmit,
  "Clear": acClear,
}

Atlas.launch(callbacks)
```
