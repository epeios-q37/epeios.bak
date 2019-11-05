# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.z_1 import *


reportErrors = True
"""
- when 'True', the validity of the values are checked upstream.
- when 'False', no check; if errors, Python exceptions are displayed.
"""


def solveFirstDegreeEquation(a, b, c):
    solution = (c-b)/a
    return str(solution)


def solveFirstDegreeInequation(a, b, ineg, c):
    sol = solveFirstDegreeEquation(a, b, c)

    if ineg == "≥":
        if a > 0:
            return "[" + sol + " ; " + "+∞["
        else:
            return "]-∞" + " ; " + sol + "]"
    if ineg == ">":
        if a > 0:
            return "]" + sol + " ; " + "+∞["
        else:
            return "]-∞" + " ; " + sol + "["
    if ineg == "≤":
        if a > 0:
            return "]-∞" + " ; " + sol + "]"
        else:
            return "[" + sol + " ; " + "+∞["
    if ineg == "<":
        if a > 0:
            return "]-∞" + " ; " + sol + "["
        else:
            return "]" + sol + " ; " + "+∞["


def solve(a, b, c, operator):
    # Only useful when 'reportErrors' at 'False'.
    if (a == 0):
        warn("'a' must be different from 0 !")
        return
    # There are other tests to do.

    erase()
    display("Solution of")
    display("{:g} × x + {:g} {} {:g}:".format(a, b, operator, c))
    if operator == '=':
        display("x = " + solveFirstDegreeEquation(a, b, c))
    else:
        display("x = " + solveFirstDegreeInequation(a, b, operator, c))


go(globals())
