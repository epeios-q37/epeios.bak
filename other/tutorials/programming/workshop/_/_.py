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
 
import os, threading, sys, inspect, traceback

_DEBUG = False

# Detecting 'Repl.it' environment.
if ('HOME' in os.environ) and (os.environ['HOME'] == '/home/runner'):
  os.environ["ATK"] = "REPLit"

sys.path.append("./Atlas.python.zip")
sys.path.append("../Atlas.python.zip")

import atlastk as Atlas

_headCommon = """
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
"""

titles = {
  "de": "Programmierungs-Workshop",
  "en": "Programming workshop",
  "fr": "Atelier de programmation"
}

_data = {}

_data['global'] = {}

def _threadId():
  return threading.currentThread().ident

def _store(set,key,value):
  globals()['_data'][set][key] = value

def _recall(set,key):
  return globals()['_data'][set][key]

def store(key, value):
  try:
    _store(_threadId(),key,value)
  except KeyError:
#    print('KE store ' + key)
    _store('global',key,value)

def recall(key):
  try:
    return _recall(_threadId(),key)
  except KeyError:
#    print('KE recall ' + key)
    return _recall('global', key)

def dom():
  return recall('dom')


def core():
  return recall('core')

_userCallback = None

class Core:
  def __init__(self, dom):
    globals()['_data'][_threadId()] = {}
    store('dom', dom)
    store('core',self)
    self.userObject = _userCallback() if _userCallback else None

def _read(path):
  return open(path).read()


def _translate(text, i18n):
  if i18n == None:
    return text

  for key in i18n:
    text = text.replace("$" + key + "$", i18n[key])

  return text


def read(fileName, i18n=None):
  return _translate(_read(fileName), i18n)


def readHTML(path, affix, i18n=None):
  return read(os.path.join('workshop', 'assets', path, affix + ".html"), i18n)


def readBody(path, i18n=None):
  return readHTML(path, "Body", i18n)


def _readHead(path, title, i18n=None):
  return "<title>" + title + "</title>\n" + _headCommon + readHTML(path, "Head", i18n)


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
		dom.alert("PYTHON EXCEPTION:\n\n" + traceback.format_exc())
		if _DEBUG:
		  raise e


def _patchWithCoreObject(userCallbacks):
  callbacks = {}

  for action in userCallbacks:
    callbacks[action] = lambda core, dom, id, action: _call(userCallbacks[action], core, dom, id, action)

  return callbacks

def _patchWithoutCoreObject(userCallbacks):
  callbacks = {}

  for action in userCallbacks:
    callbacks[action] = lambda dom, id, action: _call(userCallbacks[action], None, dom, id, action)

  return callbacks


def main(path, callback, callbacks, title, userCallback = None):
  globals()['_userCallback'] = userCallback
  Atlas.launch( _patchWithCoreObject(callbacks) if callback else _patchWithoutCoreObject(callbacks), callback, _readHead(path, title)) 
