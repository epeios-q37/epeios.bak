#!/usr/bin/python3
# coding: utf8

# Si installé avec 'pip install term2web',
# les deux lignes suivanes peuvent être retirée.
import sys, os
sys.path.append("./Atlas.zip")

import os
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
<p>Indiquez le tpe de véhicule que vous possédez :</p>
<input type="checkbox" id="bicyclette" data-xdh-onevent="Select">
<label for="bicyclette">Bicyclette</label><br>

<input type="checkbox" id="automobile" data-xdh-onevent="Select">
<label for="automobile">Automobile</label><br>

<input type="checkbox" id="pirogue" data-xdh-onevent="Select">
<label for="pirogue">Pirogue</label><br>

<fieldset>
  <span>Dernier élément cliqué : </span>
  <output id="Output"/>
</fieldset>
<div>
  <button data-xdh-onevent="Submit">Envoyer</button>
</div>
"""

def ac_connect(dom):
	dom.set_layout("", prettify(body))

def ac_select(dom,id):
  dom.set_content("Output", id +" (" + dom.get_content(id) + ")")

def ac_submit(dom):
  dom.alert(str(dom.get_contents(["bicyclette", "automobile","pirogue"])))

callbacks = {
	"": ac_connect,
  "Select": ac_select,
  "Submit": ac_submit
}

Atlas.launch(callbacks, None, head)
