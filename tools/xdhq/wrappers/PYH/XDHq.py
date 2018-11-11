""" 
  Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq If not, see <http://www.gnu.org/licenses/>.
 """

import XDHqDEMO, XDHqSHRD
from threading import Thread

def _split(keysAndValues):
	keys = []
	values = []

	for key in keysAndValues:
		keys.append(key)
		values.append(keysAndValues[key])

	return [keys,values]

def _unsplit(keys,values):
	i = 0
	keysAndValues = {}

	while i < len(keys):
		keysAndValues[keys[i]] = values[i]
		i += 1

	return keysAndValues

def _getAssetPath(dir):
	if XDHqSHRD.isDev():
		return "h:/hg/epeios/tools/xdhq/examples/common/" + dir + "/"
	else:
		return os.path.dirname(os.path.realpath(sys.path[0]))

def _getAssetFilename(path, dir):
	return _getAssetPath(dir) + path

def readAsset(path, dir=""):
	return open(_getAssetFilename(path, dir)).read()

class XDHqDOM(Thread):
	def __init__(this, userObject):
		Thread.__init__(this)
		this._dom = XDHqDEMO.XDHqDOM_DEMO()
		this._userObject = userObject

	def getAction(this):
		return this._dom.getAction()

	def _setLayout(this, id, xml, xslFilename):
		this._dom.call("SetLayout_1", XDHqSHRD.RT_VOID, 3, id, xml, xslFilename, 0)

	def setLayout(this,id,html):
		this._setLayout(id,html,"")

	def getContents(this, ids):
		return _unsplit(ids,this._dom.call("GetContents_1",XDHqSHRD.RT_STRINGS, 0, 1, ids))

	def getContent( this, id):
		return this.getContents([id])[id]

	def setContents(this,idsAndContents):
		[ids,contents] = _split(idsAndContents)

		this._dom.call("SetContents_1", XDHqSHRD.RT_VOID, 0, 2, ids, contents)

	def setContent(this, id, content):
		this.setContents({id: content})

	def _handleClasses(this, command, idsAndClasses):
		[ids, classes] = _split(idsAndClasses)

		this._dom.call(command, XDHqSHRD.RT_VOID, 0, 2, ids, classes)

	def addClasses(this, idsAndClasses):
		this._handleClasses("AddClasses_1", idsAndClasses)

	def removeClasses(this, idsAndClasses):
		this._handleClasses("RemoveClasses_1", idsAndClasses)

	def addClass(this, id, clas ):
		this.addClasses({id: clas})

	def removeClass(this, id, clas ):
		this.removeClasses({id: clas})

	def focus(this, id):
		this._dom.call("Focus_1", XDHqSHRD.RT_VOID,1, id, 0)

	def run(this):
		while True:
			[action,id] = this.getAction()
			this._userObject.handle(action,id)

def launch(newSessionAction, headContent, dir, new):
	XDHqDEMO.launch(newSessionAction,headContent)

	while True:
		new().start()






