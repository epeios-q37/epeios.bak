import sys, os

sys.path.append("./EduTK.zip")
sys.path.append("../EduTK.zip")

if ('Q37_XPP' in os.environ):
  sys.path.append(os.path.join(os.environ["HOME"],"epeios/other/libs/edutk/PYH/edutk"))

import edutk as _
from edutk import Core

F_HELLO = "Hello"
L_NAME_IS_MANDATORY = "NameIsMandatory"
L_NAME_TO_DISPLAY = "NameToDisplay"

_.defineUserItem(globals(), "uf", F_HELLO)

def clear():
  _.dom().setLayout("output", "<span/>")

def display(text):
  output = _.Atlas.createHTML()
  output.putTagAndValue("h1", text)
  _.dom().appendLayout("output", output)

def _acConnect(c, dom):
    dom.setLayout("", _.read(os.path.join("workshop", "Body.html"), c.bodyI18n))
    dom.focus("input")

def _acSubmit(c, dom):
    input=dom.getContent("input").strip()

    if (len(input)) != 0:
        ufHello()(dom.getContent("input"))
        dom.setContent( "input", "")
        dom.removeClass("output", "hidden")
    else:
        dom.alert(c.i18n[L_NAME_IS_MANDATORY])
        dom.setContent("input", "")
        dom.focus("input")
  
def main(callback, globals, userFunctionLabels, title):
  # Uncomment to display exceptions in terminal,
  # instead of being displayed in an alert box.
  # _.useRegularExceptions()
  _.assignUserItems((F_HELLO,), globals, userFunctionLabels)
  _.main( os.path.join("workshop", "Head.html"), callback,
  {
    "": _acConnect,
    "Submit": _acSubmit,
  }, title
)

 
