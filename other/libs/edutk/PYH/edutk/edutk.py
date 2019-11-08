# coding: utf-8
"""
MIT License

Copyright (c) 2019 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import os
import threading
import sys
import inspect
import traceback

# When set to true, (with 'useRegularExceptions()'),
# exceptions behave normally again,
# instead of being displayed in an alert box.
_regularExceptions = False

# Detecting 'Repl.it' environment.
if ('HOME' in os.environ) and (os.environ['HOME'] == '/home/runner'):
  os.environ["ATK"] = "REPLit"

sys.path.append("./Atlas.zip")
sys.path.append("../Atlas.zip")

import atlastk as Atlas

_USER_ITEM_LINKS = {}

_HEAD_COMMON = """
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
"""

_S_ROOT = "_storage"
_S_GLOBAL = "global"
_S_DOM = "dom"
_S_CORE = "core"

globals()[_S_ROOT] = {}

globals()[_S_ROOT][_S_GLOBAL] = {}


def _threadId():
  return threading.currentThread().ident


def _store(set, key, value):
  globals()[_S_ROOT][set][key] = value


def _recall(set, key):
  return globals()[_S_ROOT][set][key]


def store(key, value):
  try:
    _store(_threadId(), key, value)
  except KeyError:
    #    print('KE store ' + key)
    _store(_S_GLOBAL, key, value)


def recall(key):
  try:
    return _recall(_threadId(), key)
  except KeyError:
    #    print('KE recall ' + key)
    return _recall(_S_GLOBAL, key)


def dom():
  return recall(_S_DOM)


def core():
  return recall(_S_CORE)


class Core:
  def __init__(self, dom, userObject=None):
    globals()[_S_ROOT][_threadId()] = {}
    store(_S_DOM, dom)
    store(_S_CORE, self)
    self.userObject = userObject


def _read(path):
  return open(path).read()


def _translate(text, i18n):
  if i18n == None:
    return text

  for key in i18n:
    text = text.replace("$" + key + "$", i18n[key])

  return text


def read(path, i18n=None):
  return _translate(_read(path), i18n)


def _assignUserItem(label, items, name):
  if not name in items:
    return False

  _USER_ITEM_LINKS[label] = items[name]

  return True


def assignUserItem(label, items, names):
  return _assignUserItem(label, items, names[label])


def assignUserItems(labels, items, names):
  for label in labels:
    name = names[label]

    if not _assignUserItem(label, items, name):
      raise NameError("'{}' is missing!".format(name))


def defineUserItem(globals,prefix,name):
  globals[prefix + name] = lambda : _USER_ITEM_LINKS[name]


def defineUserItems(globals,prefix,names):
  for name in names:
    defineUserItem(globals,prefix, name)


def setEnums(globals,name,values):
  claz = type(name, (),{}) 

  for value in values:
    setattr(claz,value,value)

  globals[name] = claz
  

def _getHead(path, title, i18n=None):
  return "<title>" + title + "</title>\n" + _HEAD_COMMON + read(path, i18n)


# Should be the almost identical as in 'Atlas.py'
def _call(func, userObject, dom, id, action):
  amount = len(inspect.getargspec(func).args)
  args = []

  if (not(userObject)):
    amount += 1

  if (amount == 4):
    args.insert(0, action)

  if(amount >= 3):
    args.insert(0, id)

  if(amount >= 2):
    args.insert(0, dom)

  if(userObject and (amount >= 1)):
    args.insert(0, userObject)

  try:
    return func(*args)
  except Exception as e:
    if _regularExceptions:
      raise e
    else:
      dom.alert("PYTHON EXCEPTION:\n\n" + traceback.format_exc())

def _patchWithCoreObject(userCallbacks):
  callbacks = {}

  for action in userCallbacks:
    callbacks[action] = lambda core, dom, id, action: _call(
        userCallbacks[action], core, dom, id, action)

  return callbacks


def _patchWithoutCoreObject(userCallbacks):
  callbacks = {}

  for action in userCallbacks:
    callbacks[action] = lambda dom, id, action: _call(
        userCallbacks[action], None, dom, id, action)

  return callbacks

def useRegularExceptions(value = True):
    globals()["_regularExceptions"] = value;


def main(headPath, callback, callbacks, title):
  Atlas.launch(_patchWithCoreObject(callbacks) if callback else _patchWithoutCoreObject(
      callbacks), callback, _getHead(headPath, title))
