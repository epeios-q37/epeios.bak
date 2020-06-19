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
   <button data-xdh-onevent="Submit">Mon bouton!</button>
"""

def ac_connect(dom):
	dom.set_layout("", prettify(body))

def ac_submit(dom):
	dom.alert("Click bouton détecté !")

callbacks = {
	"": ac_connect,
	"Submit": ac_submit,
}

Atlas.launch(callbacks, None, head)
