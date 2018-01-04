/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of UnJS.

	UnJS is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	UnJS is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with UnJS If not, see <http://www.gnu.org/licenses/>.
*/

var unjsId = "";

if (process.env.EPEIOS_SRC) {
	if (process.platform == 'win32')
		epeiosPath = "h:/hg/epeios/"
	else
		epeiosPath = "~/hg/epeios/"

	unjsId = epeiosPath + "tools/xdhq/UnJS/NJS/UnJS.js";
} else {
	unjsId = 'unjs';
}

const unjs = require(unjsId);
const Tree = unjs.Tree;
const UnJS = unjs.UnJS;

class MyData extends UnJS {
	constructor() {
		super();
		this.timestamp = new Date();
	}
}

function newSession() {
	console.log("New session detected !");

	return new MyData();
}

function acConnect(unjs, id) {
	unjs.setLayout("", new Tree(), "Main.xsl",
		() => unjs.setCastById("Input", "Hide")
	);
}

function acSubmit(unjs, id) {
	unjs.getContent("Pattern",
		(result) => unjs.setContent("Pattern", result.toUpperCase())
	);
}

function acShowInput(unjs, id) {
	unjs.setCastById("Input", "Plain");
}

function acHideInput(unjs, id) {
	unjs.setCastById("Input", "Hide");
}

function main() {
	unjs.register([
		["Connect", acConnect],
		["Submit", acSubmit],
		["HideInput", acHideInput],
		["ShowInput", acShowInput],
	]);

	unjs.launch(newSession, "blank", "Connect");
}

main();
