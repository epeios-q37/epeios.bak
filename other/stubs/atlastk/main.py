#!/usr/bin/python3

# If installed with 'pip install atlastk',
# above line can be removed.
__import__("sys").path.append("./atlastk.zip")

import atlastk

# Replace below lines by your own code.

HEAD = """
<title>Atlas toolkit Python stub</title>
"""

BODY = """
 <fieldset>
   <button data-xdh-onevent="Submit">Click me!</button>
 </fieldset>
"""


def acConnect(dom):
	dom.inner("", BODY )


def acSubmit(dom):
	dom.alert("Hello, the World!")


CALLBACKS = {
	"": acConnect,
	"Submit": acSubmit,
}


atlastk.launch(CALLBACKS, headContent=HEAD)
