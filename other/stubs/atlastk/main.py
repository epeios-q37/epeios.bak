#!/usr/bin/python3

# If installed with 'pip install term2web',
# above line can be removed.
__import__("sys").path.append("./atlastk.zip")

import atlastk

# Replace below lines by your own code.

head = """
<title>Atlas toolkit Python stub</title>
"""

body = """
 <fieldset>
   <button data-xdh-onevent="Submit">Click me!</button>
 </fieldset>
"""


def acConnect(dom):
	dom.inner("", body )


def acSubmit(dom):
	dom.alert("Hello, the World!")


callbacks = {
	"": acConnect,
	"Submit": acSubmit,
}


atlastk.launch(callbacks, headContent)
