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

import os, sys

sys.path.append("./Atlas.python.zip")
sys.path.append("../Atlas.python.zip")

import Atlas
import pprint

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

def readAsset(path):
	return Atlas.readAsset(path, "GPIO")

def getWId(pattern):
	return int(pattern[pattern.find('.')+1:])

class GPIO:
	def __init__(this):
		this._settings = {}

	def _retrieveMode(this,wId):
		return wId % 3

	def _retrieveValue(this,wId):
		return wId % 2

	def _getSetting(this,wId):
		if not wId in this._settings:
			this._settings[wId] = {
				Setting.MODE: this._retrieveMode(wId),
				Setting.VALUE: this._retrieveValue(wId),
			}

		return this._settings[wId]

	def _getMode(this,wId):
		return this._getSetting(wId)[Setting.MODE]

	def _getValue(this,wId):
		value = this._getSetting(wId)[Setting.VALUE]

		if ( (value != 0) and (this._getMode(wId) != Mode.PWM) ):
			value = 100

		return value

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
		this._buildCorpus(xml)
		xml.pushTag("GPIOs")

		for wId in mapping:
			xml.pushTag("GPIO")
			xml.setAttribute( "id", wId)
			xml.setAttribute("Mode",this._getMode(wId))
			xml.setAttribute("Value",this._getValue(wId))
			xml.popTag()

		xml.popTag()

//		pprint.pprint(xml.toString())

		return xml

	def display(this,dom):
		dom.setLayoutXSL("GPIO", this._buildXML(), "GPIO.xsl")

	def switchMode(this,dom,wId,mode):
		id = "PWM."+str(wId);

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

def acConnect(GPIO,dom,id):
	dom.setLayout("", readAsset( "Main.html") )
	GPIO.display(dom)

def acSwitchMode(GPIO,dom,id):
	GPIO.switchMode(dom,getWId(id),int(dom.getContent(id)))
	

callbacks = {
		"Connect": acConnect,
		"SwitchMode": acSwitchMode,
	}
		
Atlas.launch("Connect", callbacks, GPIO, readAsset("Head.html"), "GPIO")
