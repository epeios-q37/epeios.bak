# coding: utf-8

"""
MIT License

Copyright (c) 2018 Claude SIMON (https://q37.info/s/rmnmqd49)

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

import XDHqDEMO, XDHqSHRD, XDHqXML

import os, sys
from collections import OrderedDict

if sys.version_info[0] == 2:
	import urllib
	def _encode(string):
		return urllib.quote(string)
elif sys.version_info[0] == 3:
	import urllib.parse
	def _encode(string):
		return urllib.parse.quote(string)
else:
	print("Unhandled python version!")
	os._exit(1)

_dir = ""

_VOID = XDHqSHRD.RT_VOID
_STRING = XDHqSHRD.RT_STRING
_STRINGS = XDHqSHRD.RT_STRINGS
XML = XDHqXML.XML

def _split(keysAndValues):
	keys = []
	values = []

	for key in keysAndValues:
		keys.append(str(key))
		values.append(str(keysAndValues[key]))

	return [keys,values]

def _unsplit(keys,values):
	i = 0
	# With 'OrderedDict', the order of the items is keeped under Python 2.
	# This facilitates the retrieving of values by using 'values()' method.
	# Dict are ordered by default under Python 3.
	keysAndValues = OrderedDict()
	length = len(keys)

	while i < length:
		keysAndValues[keys[i]] = values[i]
		i += 1

	return keysAndValues

def _getAssetPath(dir):
	if XDHqSHRD.isDev():
		return os.path.join(os.environ["Q37_EPEIOS"],"tools/xdhq/examples/common/", dir )
	else:
		return os.path.abspath(os.path.dirname(sys.argv[0]))

def _getAssetFilename(path, dir):
	return os.path.join(_getAssetPath(dir), path )

def readAsset(path, dir=""):
	return open(_getAssetFilename(path, dir)).read()

def _readXSLAsset(path, dir):
	if (path.lstrip()[0]=='<'):
		return path.lstrip()
	else:
		return readAsset(path, dir)

class DOM:
	def __init__(self,instance):
		self._dom = XDHqDEMO.DOM_DEMO(instance)

	def getAction(self):
		return self._dom.getAction()

	def execute(self,script):
		return self._dom.call("Execute_1" ,_STRING, script)

	def flush(self):	# Returns when all the pending command were executed.
		self.execute("''")

	def alert(self,message):
		self._dom.call( "Alert_1", _STRING, message )
		# For the return value being 'STRING' instead of 'VOID',
		# see the 'alert' primitive in 'XDHqXDH'.

	def confirm(self,message):
		return self._dom.call( "Confirm_1", _STRING, message ) == "true"

	def _handleLayout(self, variant, id, xml, xsl):
		#	If 'xslFilename' is empty, 'xml' contents HTML.
		# 	If 'xml' is HTML and uses the compressed form, if it has a root tag, only the children will be used.
		self._dom.call("HandleLayout_1", _VOID, variant, id, xml if isinstance(xml,str) else xml.toString(), xsl)

	def prependLayout(self,id,html):
		self._handleLayout("Prepend",id,html,"")

	def setLayout(self,id,html):
		self._handleLayout("Set",id,html,"")

	def appendLayout(self,id,html):
		self._handleLayout("Append",id,html,"")

	def _handleLayoutXSL(self, variant, id, xml, xsl):
		global _dir
		xslURL = xsl

		if True:	# Testing if 'PROD' or 'DEMO' mode when available.
			xslURL = "data:text/xml;charset=utf-8," + _encode( _readXSLAsset( xsl, _dir ) )

		self._handleLayout(variant, id, xml, xslURL )

	def prependLayoutXSL(self, id, xml, xsl):
		self._handleLayoutXSL("Prepend",id,xml,xsl)

	def setLayoutXSL(self, id, xml, xsl):
		self._handleLayoutXSL("Set",id,xml,xsl)

	def appendLayoutXSL(self, id, xml, xsl):
		self._handleLayoutXSL("Append",id,xml,xsl)

	def getContents(self, ids):
		return _unsplit(ids,self._dom.call("GetContents_1",_STRINGS, ids))

	def getContent( self, id):
		return self.getContents([id])[id]

	def setContents(self,idsAndContents):
		[ids,contents] = _split(idsAndContents)

		self._dom.call("SetContents_1", _VOID, ids, contents)

	def setContent(self, id, content):
		self.setContents({id: content})

	def setTimeout(self,delay,action ):
		self._dom.call( "SetTimeout_1", _VOID, str( delay ), action )

	"""
	# Following 4 methods will either be removed or redesigned.

	# Will become a variation of 'createElementNS(…)',
	# with a optional list of attributes.
	def createElement(self, name, id = "" ):
		return self._dom.call( "CreateElement_1", _STRING, 2, name, id, 0 )

	# Will become 'prependChild(…)', with variations.
	def insertChild(self,child,id):
		self._dom.call( "InsertChild_1", _VOID, 2, child, id, 0 )

	# NOTA: The 'CSSRule' related methods will be probably removed.
	# Enabling/disabling styles are easier to use.
	def insertCSSRule(self,rule,index,id=""):
		self._dom.call("InsertCSSRule_1", _VOID, 3, id, rule, str(index), 0)

	def appendCSSRule(self,rule,id=""):
		return int(self._dom.call("AppendCSSRule_1", _STRING, 2, id, rule, 0))

	def removeCSSRule(self,index,id=""):
		self._dom.call("RemoveCSSRule_1", _VOID, 2, id, str(index), 0)
	"""

	def dressWidgets(self,id):
		return self._dom.call( "DressWidgets_1", _VOID, id )

	def _handleClasses(self, variant, idsAndClasses):
		[ids, classes] = _split(idsAndClasses)

		self._dom.call("HandleClasses_1", _VOID, variant, ids, classes)

	def addClasses(self, idsAndClasses):
		self._handleClasses("Add", idsAndClasses)

	def removeClasses(self, idsAndClasses):
		self._handleClasses("Remove", idsAndClasses)

	def toggleClasses(self, idsAndClasses):
		self._handleClasses("Toggle", idsAndClasses)

	def addClass(self, id, clas ):
		self.addClasses({id: clas})

	def removeClass(self, id, clas ):
		self.removeClasses({id: clas})

	def toggleClass(self, id, clas ):
		self.toggleClasses({id: clas})

	def enableElements(self,ids):
		self._dom.call("EnableElements_1", _VOID, ids )

	def enableElement(self, id):
		self.enableElements([id] )

	def disableElements(self, ids):
		self._dom.call("DisableElements_1", _VOID, ids )

	def disableElement(self, id):
		self.disableElements([id])

	def setAttribute(self, id, name, value ):
		self._dom.call("SetAttribute_1", _VOID, id, name, str(value) )

	def getAttribute(self, id, name):
		return self._dom.call("GetAttribute_1", _STRING, id, name )

	def removeAttribute(self, id, name ):
		self._dom.call("RemoveAttribute_1", _VOID, id, name )

	def setProperty(self, id, name, value ):
		self._dom.call("SetProperty_1", _VOID, id, name, value )

	def getProperty(self, id, name ):
		return self._dom.call("GetProperty_1", _STRING, id, name )

	def focus(self, id):
		self._dom.call("Focus_1", _VOID, id)

def launch(callback, userCallback, callbacks, headContent, dir):
	global _dir
	_dir = dir
	XDHqDEMO.launch(callback, userCallback,callbacks,headContent)
