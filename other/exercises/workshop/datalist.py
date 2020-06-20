#!/usr/bin/python3
# coding: utf8

# Si installé avec 'pip install term2web',
# les deux lignes suivanes peuvent être retirée.
import sys
import os
# sys.path.append("./Atlas.zip")

import atlastk as Atlas

head = "<title>" + os.path.splitext(__file__)[0] + " - Atlas toolkit</title>"


def prettify(body):
    return """
<div style="display: table; margin: 50px auto auto auto;">
 <fieldset>
""" + body + """
 </fieldset>
</div>
"""

# End of header.

body = """
<div>
  <span>Parfum sélectionné :</span>
  <span id="output"/>
</div>
<div>
  <label for="parfum">Indiquez un parfum :</label>
  <input list="parfums" id="parfum" data-xdh-onevent="Submit"/>

  <datalist id="parfums">
      <option value="Caramel"/>
      <option value="Chocolat"/>
      <option value="Vanille"/>
  </datalist>
</div>
"""

parfums = ["Vanille", "Chocolat", "Caramel"]

def ac_connect(dom):
  dom.set_layout("", prettify(body))
  dom.focus("parfum")

def shape(parfums):
  html = Atlas.create_HTML()

  for parfum in parfums:
    html.push_tag("option")
    html.put_attribute("value", parfum)
    html.pop_tag()

  return html

def ac_submit(dom):
  global parfums
  parfum = dom.get_content("parfum")
  dom.set_content("parfum", "")
  if not parfum in parfums:
    parfums.append(parfum)
  parfums.sort()
  dom.set_layout("parfums", shape(parfums))
  dom.set_content("output", parfum)

callbacks = {
	"": ac_connect,
  "Submit": ac_submit
}

Atlas.launch(callbacks, None, head)
