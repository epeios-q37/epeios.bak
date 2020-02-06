from distutils.core import setup, Extension
setup(name='pyhq', version='1.0',  \
      ext_modules=[Extension('pyhq', ['pyhq.cpp'])])
