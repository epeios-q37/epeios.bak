/*
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
*/

"use strict";

var atlas;

if (process.env.EPEIOS_SRC) {
	let epeiosPath = "";

	if (process.platform === 'win32')
		epeiosPath = "h:/hg/epeios/";
	else
		epeiosPath = "~/hg/epeios/";

	atlas = require(epeiosPath + "tools/xdhq/Atlas/NJS/Atlas.js");
} else {
	atlas = require('atlastk');
}

const DOM = atlas.DOM;

const readAsset = atlas.readAsset;

class Puzzle extends DOM {
}

function fill(dom) {
	let numbers = [];
	let contents = [];

	for (let i = 0; i < 16; i++)
		numbers.push(i);

	for (let i = 0; i < 16; i++) {
		let number = numbers.splice(Math.floor(Math.random() * numbers.length), 1)[0];

		if (number !== 0)
			contents["t" + i] = number;
		else
			dom.blank = i;
	}

	dom.setContents(contents);
	dom.toggleClass(dom.blank, "hidden");
}

function drawSquare(xml, x, y) {
	xml.pushTag("use");
	xml.setAttribute("id", y * 4 + x);
	xml.setAttribute("data-xdh-onevent", "Swap");
	xml.setAttribute("x", x * 100 + 24);
	xml.setAttribute("y", y * 100 + 24);
	xml.setAttribute("xlink:href", "#stone");
	xml.popTag();

	return xml;
}

function drawGrid(dom) {
	let xml = atlas.createXML("g");

	for (let x = 0; x < 4; x++)
		for (let y = 0; y < 4; y++)
			xml = drawSquare(xml, x, y);

	dom.setLayout("Stones", xml);
}

function setText(xml, x, y) {
	xml.pushTag("tspan");
	xml.setAttribute("id", "t" + (y * 4 + x));
	xml.setAttribute("x", x * 100 + 72);
	xml.setAttribute("y", y * 100 + 90);
//	xml.setValue(x + y * 4);
	xml.popTag();

	return xml;
}

function setTexts(dom) {
	let xml = atlas.createXML("text");

	for (let x = 0; x < 4; x++)
		for (let y = 0; y < 4; y++)
			xml = setText(xml, x, y);

	dom.setLayout("Texts", xml);
}

function swap(dom, source) {
	dom.getContent(
		"t" + source,
		(value) => dom.setContents({
				["t" + dom.blank]: value,
				["t" + source]: ""
			},
			() => dom.toggleClasses({
				[dom.blank]: "hidden",
				[source]: "hidden"
				},
				() => dom.blank = source)));
}

function scramble(dom) {
	drawGrid(dom);
	setTexts(dom);
	fill(dom);
}

function acConnect(dom, id) {
	dom.setLayout("", readAsset("Main.html"));
	scramble(dom);
}

function acSwap(dom, id) {
	id = parseInt(id);
	if ([id - 1, id + 1, id + 4, id - 4].includes(dom.blank))
		swap(dom, id);
}

function newSession() {
	return new Puzzle();
}

function main() {
	const callbacks = {
		"": acConnect,
		"Swap": acSwap,
		"Scramble": (dom,id) => scramble(dom)
	};

	atlas.launch(newSession, callbacks, readAsset("Head.html"));
}

main();
