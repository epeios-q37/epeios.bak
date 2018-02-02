/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

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

/*
This is the Node.js UnJSq (http://q47.info/UnJSq/) version
of the TodoMVC (http://todomvc.com) project
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
		this.exclude = null;
		this.todos = [
			{
				"completed": true,
				"label": "Note 1"
			},  {
				"completed": false,
				"label": "Note 2"
			}
		];
//		this.todos = [];
	}
	itemsLeft() {
		var i = this.todos.length;
		var count = 0;

		while (i--) {
			if (!this.todos[i]['completed'])
				count++;
		}

		return count;
	}
}

function push(todo, id, tree) {
	var i = 0;
	tree.pushTag('Todo');
	tree.putAttribute('id', id);
	if ( todo["completed"])
		tree.putAttribute( "completed", "true");
	else
		tree.putAttribute( "completed", "false");

	tree.putValue(todo["label"]);
	tree.popTag();
}

function displayCount(dom, count) {
	switch (count) {
	case 0:
		dom.setContent("Count", "");
		break;
	case 1:
		dom.setContent("Count", "1 item left");
		break;
	default:
		dom.setContent("Count", dom.itemsLeft() + " items left");
		break;
	}
}

function handleCount(dom) {
	var count = dom.itemsLeft();

	if ( count != dom.todos.length )
		dom.disableElement( "HideClearCompleted",
			() => displayCount(dom, count))
	else
		dom.enableElement("HideClearCompleted",
			() => displayCount(dom, count))
}

function displayTodos(dom) {
	var tree = new Tree();
	var i = 0;

	tree.pushTag("Todos");

	while (i < dom.todos.length) {
		if ( dom.exclude === null || ( dom.todos[i]['completed'] != dom.exclude ) )
			push(dom.todos[i], i, tree);
		i++;
	}

	tree.popTag();

	dom.setLayout("Todos", tree, "Todos.xsl",
		() => handleCount( dom )
	);
}

function newSession() {
	console.log("New session detected !");

	return new MyData();
}

function acConnect(dom, id) {
	dom.setLayout("", new Tree(), "Main.xsl",
		() => displayTodos(dom)
	);
}

function acSubmit(dom, id) {
	dom.getContent(id,
		(content) => dom.setContent(id, "",
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

function acDestroy(dom, id) {
	dom.getContent(id,
		(content) => {
			dom.todos.splice(parseInt(content), 1);
			displayTodos(dom);
		}
	);
}

function acToggle(dom, id) {
	var i = parseInt( id );

	dom.todos[i]['completed'] = !dom.todos[i]['completed'];

	dom.toggleClass("Todo." + id, "completed",
		() => {
			if (dom.exclude != null)
				displayTodos(dom);
			else
				handleCount(dom);
		}
	);
}

function acAll(dom, id) {
	dom.exclude = null;
	dom.addClass( "All", "selected",
		() => dom.removeClasses(
			{
				"Active": "selected",
				"Complete": "selected"
			},
			() => displayTodos(dom)
		)
	)
}

function acActive(dom, id) {
	dom.exclude = true;
	dom.addClass("Active", "selected",
		() => dom.removeClasses(
			{
				"All": "selected",
				"Complete": "selected"
			},
			() => displayTodos(dom )
		)
	)
}

function acComplete(dom, id) {
	dom.exclude = false;
	dom.addClass("Complete", "selected",
		() => dom.removeClasses(
			{
				"All": "selected",
				"Active": "selected"
			},
			() => displayTodos(dom)
		)
	)
}

function acClear(dom, id) {
	var i = 0;

	while (i < dom.todos.length) {
		if (dom.todos[i]['completed'] )
			dom.todos.splice(i, 1);
		else
			i++;
	}

	displayTodos(dom);

}

function acEdit(dom, id) {
	dom.getContent( id,
		( content ) => dom.addClasses(
				{
					["View." + content]: "hide",
					[id]: "editing"
				},
				() => dom.setContent("Input." + content, dom.todos[parseInt(content)]['label']
			)
		)
	);
}

function main() {
	unjsq.register(
		{
			"Connect": acConnect,
			"Submit": acSubmit,
			"Destroy": acDestroy,
			"Toggle": acToggle,
			"All": acAll,
			"Active": acActive,
			"Complete": acComplete,
			"Clear": acClear,
			"Edit": acEdit,
		}
	);

	unjsq.launch(newSession, "Connect");
}

main();
