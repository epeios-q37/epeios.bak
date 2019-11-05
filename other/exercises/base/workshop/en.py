import workshop.common as _

class _Core(_.Core):
    i18n = {
      _.L_NAME_IS_MANDATORY: "The name is mandatory!"
    }
    bodyI18n = {
      _.L_NAME_TO_DISPLAY: "You first name"
    }

    def __init__(self, dom):
        _.Core.__init__(self, dom)
        
        
def erase():
  _.clear()

def display(text):
  _.display(text)        

def go(globals):
    _.main(lambda dom: _Core(dom), globals, {_.F_HELLO: "displayGreetings"}, "Programming workshop") 
