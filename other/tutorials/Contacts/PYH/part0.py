# Only purpose is to display all the element of the HTML page.

import atlastk

def ac_connect(dom):
  dom.inner("",open("Main.html").read())
  dom.disable_elements(("HideDisplay","HideDisplayAndSelect","HideEdition"))

CALLBACKS = {
  "": ac_connect,
 }

atlastk.launch(CALLBACKS,None,open("Head.html").read())