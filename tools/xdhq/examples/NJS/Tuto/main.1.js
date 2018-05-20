/*
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
*/

"use strict"

var atlasId = "";

if (process.env.EPEIOS_SRC) {
	let epeiosPath = "";

	if (process.platform == 'win32')
		epeiosPath = "h:/hg/epeios/"
	else
		epeiosPath = "~/hg/epeios/"

	atlasId = epeiosPath + "tools/xdhq/Atlas/NJS/Atlas.js";
} else {
	atlasId = 'atlastk';
}

const atlas = require(atlasId);
const Tree = atlas.Tree;
const DOM = atlas.DOM;

class MyData extends DOM {
	constructor() {
		super();
		this.exclude = null;
		this.index = -1;
		this.todos = [];
	}
}

function newSession() {
	console.log("New session detected !");

	return new MyData();
}

function acConnect(dom, id) {
	dom.setLayout("", new Tree(), "Main.1.xsl");
}

function push(todo, id, tree) {
	tree.pushTag('Todo');
	tree.putAttribute('id', id);

	tree.putValue(todo["label"]);
	tree.popTag();
}

function displayTodos(dom) {
	var tree = new Tree();
	var i = 0;

	dom.index = -1;

	tree.pushTag("Todos");

	while (i < dom.todos.length) {
		push(dom.todos[i], i, tree);
		i++;
	}

	tree.popTag();

	dom.setLayout("Todos", tree, "Todos.1.xsl"	);
}

function acSubmit(dom, id) {
	dom.getContent("Input",
		(content) => dom.setContent("Input", "",
			() => {
				if (content.trim() != "") {
					dom.todos.unshift(
						{
							"completed": false,
							"label": content
						}
					);
					displayTodos(dom);
				}
			}
		)
	);
}

function main() {
	atlas.register(
		{
			"Connect": acConnect,
			"Submit": acSubmit,
		}
	);

	atlas.launch(newSession, "Connect");
}

main();
