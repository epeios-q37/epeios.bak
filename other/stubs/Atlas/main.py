#!/usr/bin/python3

# If installed with 'pip install term2web',
# both above lines can be removed.
import sys
sys.path.append("./Atlas.zip")

import atlastk as Atlas

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


Atlas.launch(callbacks, None, head)
