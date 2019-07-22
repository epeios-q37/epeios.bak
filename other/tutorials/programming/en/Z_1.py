# coding: utf-8

import sys
sys.path.append(".")
from workshop.en.z_1 import *


handleErrors = True
"""
- when 'True', the validuty of the values are checked upstream.
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


def myFunction(a, b, c, operator):
    # Only useful when 'handleErrore' at 'Tru'.
    if (a == 0):
        warn("'a' must be different from 0 !")
        return
    # There are other tests to do.

    erase()
    display("Solution of")
    display("{} × x + {} {} {} :".format(a, b, operator, c))
    if operator == '=':
        display("x = " + solveFirstDegreeEquation(a, b, c))
    else:
        display("x = " + solveFirstDegreeInequation(a, b, operator, c))


go(myFunction, handleErrors)
