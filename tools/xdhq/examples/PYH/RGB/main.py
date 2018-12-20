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

import os, pprint, sys
import wiringpi

sys.path.append("./Atlas.python.zip")
sys.path.append("../Atlas.python.zip")
sys.path.append("./Atlas")

import Atlas

def readAsset(path):
	return Atlas.readAsset(path, "RGB")

class RGB:
	def __init__(this):
		pass

def acConnect(RGB,dom,id):
	dom.setLayout("", readAsset( "Main.html") )

def convert(hex):
	return int(hex,16) * 100 / 256

def acSelect(RGB, dom, id):
	R = convert(id[0:2])
	V = convert(id[2:4])
	B = convert(id[4:6])
	print (R, V, B)
	wiringpi.softPwmWrite(12,R)
	wiringpi.softPwmWrite(13,V)
	wiringpi.softPwmWrite(14,B)

callbacks = {
		"": acConnect,
		"Select": acSelect,
	}

wiringpi.wiringPiSetup()

wiringpi.softPwmCreate(13,0,100)
wiringpi.softPwmCreate(14,0,100)
wiringpi.softPwmCreate(15,0,100)

Atlas.launch(callbacks, RGB, readAsset("Head.html"), "RGB")
