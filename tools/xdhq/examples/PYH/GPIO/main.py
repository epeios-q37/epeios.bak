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

import os, pprint, sys, threading

sys.path.append("./Atlas.python.zip")
sys.path.append("../Atlas.python.zip")

import Atlas

lock = threading.Lock()

availableUserId = 0
currentUserId = None
settings = {}

mappings = {
	"Odroid C2":
	{
		0: {},
		1: {},
		2: {},
		3: {}
	}
}

mapping = mappings["Odroid C2"]

class Setting:
	MODE = 0
	VALUE = 1

class Mode:
	IN = 0
	OUT = 1
	PWM = 2
	label = {
		IN: "IN",
		OUT: "OUT",
		PWM: "PWM",
	}

def getGPIOElements():
	elements = []

	for key in mapping:
		elements.append("Mode." + str(key))
		elements.append("Value." + str(key))

	return elements

def getNewUserId():
	global availableUserId, lock

	lock.acquire()

	userId = availableUserId
	availableUserId += 1

	lock.release()

	return userId

def getCurrentUserId():
	global currentUserId

	return currentUserId

def setCurrentUserId(id):
	global currentUserId, lock

	lock.acquire()

	wasMe = currentUserId == id

	currentUserId = id

	lock.release()

	return wasMe


def set(userId,wId,field,value):
	global settings, lock
	if (getCurrentUserId() == userId):
		lock.acquire()
		settings[wId][field] = value
		lock.release()
		return True
	else:
		return False

def retrieveMode(wId):
	return wId % 3

def retrieveValue(wId):
	return wId % 2

def retrieveSetting(wId):
	return {
		Setting.MODE: retrieveMode(wId),
		Setting.VALUE: retrieveValue(wId),
	}

def retrieveSettings():
	settings = {}

	for key in mapping:
		settings[key] = retrieveSetting(key)

	return settings

def syncSettings():
	global settings, lock

	lock.acquire()

	settings = retrieveSettings()

	lock.release()

def readAsset(path):
	return Atlas.readAsset(path, "GPIO")

def getWId(pattern):
	return int(pattern[pattern.find('.')+1:])

class GPIO:
	def __init__(this):
		this._userId = getNewUserId()
		this._settings = {}
# 	setCurrentUserId(this._userId)	To early ! Must be done at connection !

	def _amIOwner(this):
		return getCurrentUserId() == this._userId

	def updateAccessibility(this,dom):
		if (this._amIOwner()):
			dom.addClass("Acquire", "hidden")
			dom.enableElements(getGPIOElements())
		else:
			dom.removeClass("Acquire", "hidden")
			dom.disableElements(getGPIOElements())


	def _set(this,dom,field,wId,mode):
		if (not(set(this._userId, field, wId, mode))):
			dom.alert( "State externally modified: updating!")
			this.display(dom)
			return False
		else:
			return True

	def _getSetting(this,wId):
		global settings

		return settings[wId]

	def _getMode(this,wId):
		return this._getSetting(wId)[Setting.MODE]

	def _setMode(this,dom,wId,mode):
		return this._set(dom,wId,Setting.MODE, mode)

	def _getValue(this,wId):
		value = this._getSetting(wId)[Setting.VALUE]

		if ( (value != 0) and (this._getMode(wId) != Mode.PWM) ):
			value = 100

		return value

	def _setValue(this,dom,wId,value):
		return this._set(dom,wId,Setting.VALUE,value)

	def _getModeLabel(this,wId):
		return Mode.label[this._getSetting(wId)[Setting.MODE]]

	def _buildModeCorpus(this,xml):
		xml.pushTag("Modes")

		for wId in Mode.label:
			xml.pushTag("Mode")
			xml.setAttribute("id", wId)
			xml.setAttribute("Label", Mode.label[wId])
			xml.popTag()

		xml.popTag()

	def _buildCorpus(this,xml):
		xml.pushTag( "Corpus")

		this._buildModeCorpus(xml)

		xml.popTag()

	def _buildXML(this):
		global mapping
		xml = Atlas.createXML("XDHTML")
		xml.setAttribute("IsOwner",this._amIOwner())
		this._buildCorpus(xml)
		xml.pushTag("GPIOs")

		for wId in mapping:
			xml.pushTag("GPIO")
			xml.setAttribute( "id", wId)
			xml.setAttribute("Mode",this._getMode(wId))
			xml.setAttribute("Value",this._getValue(wId))
			xml.popTag()

		xml.popTag()

#		pprint.pprint(xml.toString())

		return xml

	def take(this):
		setCurrentUserId(this._userId)

	def sync(this,dom):
		if ( not(this._amIOwner())):
			this.updateAccessibility(dom)
			this.display(dom)

	def display(this,dom):
		dom.setLayoutXSL("GPIO", this._buildXML(), "GPIO.xsl")

	def setMode(this,dom,wId,mode):
		id = "Value."+str(wId);

		if (this._setMode(dom,wId,mode)):
			dom.setAttribute(id,"value",this._getValue(wId))

			if (mode==Mode.IN):
				dom.disableElement(id)
				dom.setAttribute(id,"step","100")
			elif (mode==Mode.OUT):
				dom.enableElement(id)
				dom.setAttribute(id,"step","100")
			elif (mode==Mode.PWM):
				dom.enableElement(id)
				dom.setAttribute(id,"step","1")
			else:
				sys.exit("???")

	def setValue(this,dom,wId,value):
		if (this._setValue(dom,wId,value)):
			pass

def acConnect(GPIO,dom,id):
	dom.setLayout("", readAsset( "Main.html") )
	GPIO.take()
	GPIO.display(dom)
	GPIO.updateAccessibility(dom)
	dom.setTimeout(1000,"Sync")

def acSwitchMode(GPIO,dom,id):
	GPIO.setMode(dom,getWId(id),int(dom.getContent(id)))
	
def acChangeValue(GPIO,dom,id):
	GPIO.setValue(dom,getWId(id),int(dom.getContent(id)))

def acSync(GPIO,dom,id):
	GPIO.sync(dom)
	dom.setTimeout(1000,"Sync")

def acAcquire(GPIO,dom,id):
	GPIO.take()
	GPIO.updateAccessibility(dom)

callbacks = {
		"Connect": acConnect,
		"Sync": acSync,
		"Acquire": acAcquire,
		"SwitchMode": acSwitchMode,
		"ChangeValue": acChangeValue,
	}

syncSettings()
		
Atlas.launch("Connect", callbacks, GPIO, readAsset("Head.html"), "GPIO")
