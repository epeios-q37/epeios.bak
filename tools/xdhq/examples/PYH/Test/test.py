import os, sys, time

os.chdir(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("../../atlastk")

import atlastk as Atlas

def acConnect(dom):
    dom.inner("", open("Main.html").read())
    dom.scroll_to(dom.last_child("Main"))
    dom.toggle_classes( {
        dom.first_child("Main"): "test",
        dom.previous_sibling("Middle"): "test",
        dom.next_sibling("Middle"): "test"
    })
    # dom.flush()
    dom.scroll_to(dom.first_child("Main"))    

callbacks = {
	"": acConnect,
}

Atlas.launch(callbacks, None, open("Head.html").read())