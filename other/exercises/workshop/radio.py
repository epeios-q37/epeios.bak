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
# Pour présélectionner un élement : 'checked="checked"'

body = """
<p>Veuillez choisir la meilleure méthode pour vous contacter :</p>
<div>
  <input type="radio" name="contact" id="email" data-xdh-onevent="Select">
  <label for="email">Email</label>

  <input type="radio" name="contact" id="telephone" data-xdh-onevent="Select">
  <label for="telephone">Téléphone</label>

  <input type="radio" name="contact" id="courrier" data-xdh-onevent="Select">
  <label for="courrier">Courrier</label>
</div>
<fieldset>
  <span>Élément sélectionné : </span><span id="Output"/>
</fieldset>
<div>
  <button data-xdh-onevent="Submit">Envoyer</button>
</div>
"""

def ac_connect(dom):
	dom.set_layout("", prettify(body))

def ac_select(dom,id):
  dom.set_content("Output", id)

def ac_submit(dom):
  dom.alert(str(dom.get_contents(["email", "telephone","courrier"])))

callbacks = {
	"": ac_connect,
  "Select": ac_select,
  "Submit": ac_submit
}

Atlas.launch(callbacks, None, head)
