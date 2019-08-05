import sys, os

sys.path.append("./EduTK.python.zip")
sys.path.append("../EduTK.python.zip")

if ('EPEIOS_SRC' in os.environ):
  sys.path.append("/cygdrive/h/hg/epeios/other/libs/edutk/PYH/edutk")

from edutk import *

import edutk

# Uncomment for execptions behaving normally again
# instead of being displayed in a alert box.
# edutk.regularException = True
