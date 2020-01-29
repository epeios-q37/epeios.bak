#!/usr/bin/python3

# If installed with 'pip install term2web',
# both above lines can be removed.
import sys
sys.path.append("./term2web.zip")

from term2web import *

# Replace below lines by your own code.

name = input("What's your name? ")

print( "Nice to meet you, ", end="")

set_properties({
    "font-style": "italic",
    "font-size": "larger",
    "color": "salmon",
    "outline-style": "dashed",
    "outline-color": "blue"
})

print(" " + name+ " ")

