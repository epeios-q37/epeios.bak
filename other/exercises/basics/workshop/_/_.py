import sys, os

sys.path.append("./EduTK.python.zip")
sys.path.append("../EduTK.python.zip")

if ('EPEIOS_SRC' in os.environ):
  sys.path.append("/cygdrive/h/hg/epeios/other/libs/edutk/PYH/edutk")

import edutk as _

class Core(_.Core):
  pass

# Uncomment for exceptions behaving normally again
# instead of being displayed in a alert box.
# _.regularException = True

F_MY_FUNCTION = "MyFunction"

def _defineUserFunction(name):
  _.defineUserFunction(globals(), "uf" + name)

_defineUserFunction(F_MY_FUNCTION)


def mainBase(folder, callback, callbacks, ids, globals, userFunctionLabels, title):
  _.setUserFunctions(ids, globals, userFunctionLabels)
  _.main(folder, callback, callbacks, title)
