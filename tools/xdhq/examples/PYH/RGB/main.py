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

RPin = 13
GPin = 14
BPin = 21

def readAsset(path):
	return Atlas.readAsset(path, "RGB")

class RGB:
	def __init__(this):
		pass

def acConnect(RGB,dom,id):
	dom.setLayout("", readAsset( "Main.html") )

def convert(hex):
	return int(int(hex,16) * 100 / 256)

def acSelect(RGB, dom, id):
	global RPin, GPin, BPin
	R = convert(id[0:2])
	G = convert(id[2:4])
	B = convert(id[4:6])
	print (R, G, B)
	wiringpi.softPwmWrite(RPin,100 - R)
	wiringpi.softPwmWrite(GPin,100 - G)
	wiringpi.softPwmWrite(BPin,100 - B)

callbacks = {
		"": acConnect,
		"Select": acSelect,
	}

wiringpi.wiringPiSetup()

wiringpi.softPwmCreate(RPin,0,100)
wiringpi.softPwmCreate(GPin,0,100)
wiringpi.softPwmCreate(BPin,0,100)

Atlas.launch(callbacks, RGB, readAsset("Head.html"), "RGB")
