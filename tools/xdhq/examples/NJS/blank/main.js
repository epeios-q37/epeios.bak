/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of UnJSq.

	UnJSq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	UnJSq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with UnJSq If not, see <http://www.gnu.org/licenses/>.
*/

"use strict"

var unjsqId = "";

if (process.env.EPEIOS_SRC) {
	let epeiosPath = "";

	if (process.platform == 'win32')
		epeiosPath = "h:/hg/epeios/"
	else
		epeiosPath = "~/hg/epeios/"

	unjsqId = epeiosPath + "tools/xdhq/UnJSq/NJS/UnJSq.js";
} else {
	unjsqId = 'unjsq';
}

const unjsq = require(unjsqId);
const Tree = unjsq.Tree;
const DOM = unjsq.DOM;

class MyData extends DOM {
	constructor() {
		super();
		this.timestamp = new Date();
	}
}

function newSession() {
	console.log("New session detected !");

	return new MyData();
}

function acConnect(dom, id) {
	dom.setLayout("", new Tree(), "Main.xsl",
		() => dom.addClass("Input", "hidden")
	);
}

function acSubmit(dom, id) {
	dom.getContent("Pattern",
		(result) => dom.setContent("Pattern", result.toUpperCase())
	);
}

function acShowInput(dom, id) {
	dom.removeClass("Input", "hidden");
}

function acHideInput(dom, id) {
	dom.addClass("Input", "hidden");
}

function main() {
	unjsq.register([
		["Connect", acConnect],
		["Submit", acSubmit],
		["HideInput", acHideInput],
		["ShowInput", acShowInput],
	]);

	unjsq.launch(newSession, "Connect");
}

main();
