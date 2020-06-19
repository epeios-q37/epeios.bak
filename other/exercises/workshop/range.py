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
<div>
  <label for="volume">Volume</label>
  <input type="range" id="volume" min="0" max="10" data-xdh-onevent="Slide">

  <meter id="fuel"
       min="0" max="10"
       low="5" high="8" optimum="4"
       value="5" step="2">
	</meter>
</div>
"""

def ac_connect(dom):
	dom.set_layout("", prettify(body))
	dom.focus("volume")

def ac_slide(dom):
	dom.set_attribute("fuel", "value", (dom.getContent("volume")))

callbacks = {
	"": ac_connect,
	"Slide": ac_slide
}

Atlas.launch(callbacks, None, head)
