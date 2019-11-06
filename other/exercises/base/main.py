# coding: utf-8

# All files under MIT license
# Copyright (c) 2019 Claude SIMON (https://q37.info/s/rmnmqd49)
# See  'LICENSE' file.

lang = ""

try:
    input = raw_input
except NameError:
    pass

while ( ( lang != '1' ) and ( lang != "2" ) ):
    lang = input("1: English; 2: Français ? (1) ") or "1"
    
if ( lang == '1' ):
    from en import *
elif ( lang == '2' ):
    from fr import *


