""" 
 	Copyright (C) 2019 Claude SIMON (http://q37.info/contact/).

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

import sys
from random import *

sys.path.append("./Atlas.python.zip")
sys.path.append("../Atlas.python.zip")

import atlastk as Atlas

class Tiles:
	pass

def readAsset(path):
	return Atlas.readAsset(path, "Tiles")

def fill(tiles,dom):
	numbers = []
	contents = {}

	for i in range(16):
		numbers.append(i)

	for i in range(len(numbers)):
		number = numbers.pop(randint(0,len(numbers)-1))
		if number != 0:
			contents["t"+str(i)]=number
		else:
			tiles.blank=i

	dom.setContents(contents)

def swap(tiles,dom,source):
	dom.setContents({
		"t"+str(tiles.blank): dom.getContent("t"+str(source)),
		"t"+str(source): ""
	})

	tiles.blank=source


def drawSquare(xml,x,y):
	xml.pushTag("rect")
	xml.setAttribute("id", y * 4 + x)
	xml.setAttribute("data-xdh-onevent","Swap")
	xml.setAttribute("x",x * 65 + 1)
	xml.setAttribute("y",y * 65 + 1)
	xml.setAttribute("width",60)
	xml.setAttribute("height",60)
	xml.popTag()

def drawGrid(xml):
	xml.pushTag("g")
	xml.setAttribute("style", "fill-opacity: 0; stroke: black;")
	for x in range(0,4):
		for y in range(0,4):
			drawSquare(xml,x,y)
	xml.popTag()

def setText(xml,x,y):
	xml.pushTag("Text")
	xml.setAttribute("id", "t" + str(y * 4 + x))
	xml.setAttribute("style", "dominant-baseline: central;")
	xml.setAttribute("x", x * 65 + 30 )
	xml.setAttribute("y", y * 65 + 30 )
	xml.popTag()

def setTexts(xml):
	xml.pushTag("g")
	xml.setAttribute("style", "font-size: 30px; text-anchor: middle;")
	for x in range(0,4):
		for y in range(0,4):
			setText(xml,x,y)
	xml.popTag()

def acConnect(this,dom,id):
	dom.setLayout("", readAsset( "Main.html"))
	xml = Atlas.createXML("g")
	xml.setAttribute("xmlns","http://www.w3.org/2000/svg")
	setTexts(xml)
	drawGrid(xml)
	dom.setLayout("Grid", xml.toString())
	fill(this,dom)

def acSwap(this,dom,id):
	id = int(id)
	if this.blank in [id+1, id-1, id+4, id-4]:
		swap(this,dom,id)

callbacks = {
	"": acConnect,
	"Swap": acSwap,
}

Atlas.launch(callbacks, Tiles, "", "Tiles")