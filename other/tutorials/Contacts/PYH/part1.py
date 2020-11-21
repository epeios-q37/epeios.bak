import atlastk

def ac_connect(dom):
  dom.inner("",open("Main.html").read())

CALLBACKS = {
  "": ac_connect,
 }

atlastk.launch(CALLBACKS,None,open("Head.html").read())