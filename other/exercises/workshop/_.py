#!/usr/bin/python3
# coding: utf8

# Si installé avec 'pip install term2web',
# les deux lignes suivanes peuvent être retirée.
import sys, os
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
"""

def ac_connect(dom):
	dom.set_layout("", prettify(body))

callbacks = {
	"": ac_connect,
}

Atlas.launch(callbacks, None, head)
