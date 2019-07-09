# coding: utf-8

import atlastk as Atlas
import os
import threading
import sys
import inspect

if ('HOME' in os.environ) and (os.environ['HOME'] == '/home/runner'):
  os.environ["ATK"] = "REPLit"

sys.path.append("./Atlas.python.zip")
sys.path.append("../Atlas.python.zip")

_headCommon = """
<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
"""

titles = {
  "de": "Programmierungs-Workshop",
  "en": "Programming workshop",
  "fr": "Atelier de programmation"
}

_data = {}


def _threadId():
  return threading.currentThread().ident


# Does not work as static member of '_Core' with Python 2!
_userCallback = None


class _Core:
  def __init__(self):
    globals()['_data'][_threadId()] = {}
    self.userObject = _userCallback()


def store(key, value):
  globals()['_data'][_threadId()][key] = value


def recall(key):
  return globals()['_data'][_threadId()][key]


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


def _acPreProcessing(core, dom, action, id):
  core.action = action

  return True


def _call(func, userObject, dom, id, action):
	amount = len(inspect.getargspec(func).args)
	args = []

	if (amount == 4):
		args.insert(0, action)

	if(amount >= 3):
		args.insert(0, id)

	if(amount >= 2):
		args.insert(0, dom)

	if(amount >= 1):
		args.insert(0, userObject)

	try:
		return func(*args)
	except Exception as e:
		dom.alert("PYTHON ERROR: \n\n" + str(e))


def _patch(userCallbacks):
  callbacks = {}

  for action in userCallbacks:
    callbacks[action] = lambda core,dom,id,action: _call(userCallbacks[action],core.userObject,dom,id,action)

  return callbacks

def main(path,callback,callbacks,title = None):
  globals()['_userCallback'] = callback
  Atlas.launch( _patch(callbacks), _Core, _readHead(path, title)) 
