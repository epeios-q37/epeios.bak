import workshop.common as _

class _Core(_.Core):
    i18n = {
      _.label.MissingName: "Please enter a first name!"
    }
    body_i18n = {
      _.label.NameToDisplay: "First name"
    }

    def __init__(self, dom):
        _.Core.__init__(self, dom)
        
        
erase = _.clear
display = _.display

def go(globals):
    _.main(lambda dom: _Core(dom), globals, {_.F_HELLO: "displayGreetings"}, "Programming workshop") 
