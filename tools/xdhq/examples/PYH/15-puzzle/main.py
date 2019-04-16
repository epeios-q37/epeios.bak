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

class Puzzle:
	pass

def readAsset(path):
	return Atlas.readAsset(path, "15-puzzle")

def fill(puzzle,dom):
	numbers = []
	contents = {}

	for i in range(16):
		numbers.append(i)

	for i in range(len(numbers)):
		number = numbers.pop(randint(0,len(numbers)-1))
		if number != 0:
			contents["t"+str(i)]=number
		else:
			puzzle.blank=i

	dom.setContents(contents)
	dom.toggleClass(puzzle.blank,"hidden")

def swap(puzzle,dom,source):
	dom.setContents({
		"t"+str(puzzle.blank): dom.getContent("t"+str(source)),
		"t"+str(source): ""
	})

	dom.toggleClasses({
		puzzle.blank: "hidden",
		source: "hidden"
	})

	puzzle.blank=source

def drawSquare(xml,x,y):
	xml.pushTag("use")
	xml.setAttribute("id", y * 4 + x)
	xml.setAttribute("data-xdh-onevent","Swap")
	xml.setAttribute("x",x * 100 + 24)
	xml.setAttribute("y",y * 100 + 24)
	xml.setAttribute("xlink:href","#stone")
	xml.popTag()

def drawGrid(dom):
	xml = Atlas.createXML("g")
	for x in range(0,4):
		for y in range(0,4):
			drawSquare(xml,x,y)
	dom.setLayout("Stones",xml)

def setText(xml,x,y):
	xml.pushTag("tspan")
	xml.setAttribute("id", "t" + str(y * 4 + x))
	xml.setAttribute("x", x * 100 + 72 )
	xml.setAttribute("y", y * 100 + 90 )
	xml.popTag()

def setTexts(dom):
	xml = Atlas.createXML("text")
#	xml.setAttribute("style", "font-size: 30px; text-anchor: middle;")
	for x in range(0,4):
		for y in range(0,4):
			setText(xml,x,y)
	dom.setLayout("Texts",xml)

def scramble(puzzle,dom):
	xml = Atlas.createXML("g")
	xml.setAttribute("xmlns","http://www.w3.org/2000/svg")
	drawGrid(dom)
	setTexts(dom)
#	dom.setLayout("Grid", xml.toString())
	fill(puzzle,dom)

def acConnect(this,dom,id):
	dom.setLayout("", readAsset( "Main.html"))
	scramble(this,dom)

def acSwap(this,dom,id):
	id = int(id)
	if this.blank in [id+1, id-1, id+4, id-4]:
		swap(this,dom,id)

def acScramble(this,dom,id):
	scramble(this,dom)

callbacks = {
	"": acConnect,
	"Swap": acSwap,
	"Scramble": acScramble
}

Atlas.launch(callbacks, Puzzle, readAsset("Head.html"), "15-puzzle")