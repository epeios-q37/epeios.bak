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
of the TodoMVC application (http://todomvc.com/).
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
		this.id = -1;
		/*		
				this.todos = [
					{
						"completed": true,
						"label": "Note 1"
					}, {
						"completed": false,
						"label": "Note 2"
					}
				];
		*/
		this.todos = [];
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
	tree.pushTag('Todo');
	tree.putAttribute('id', id);

	if (todo["completed"])
		tree.putAttribute("completed", "true");
	else
		tree.putAttribute("completed", "false");

	tree.putValue(todo["label"]);
	tree.popTag();
}

function displayCount(dom, count) {
	var text = "";

	switch (count) {
		case 0:
			break;
		case 1:
			text = "1 item left";
			break;
		default:
			text = dom.itemsLeft() + " items left";
			break;
	}

	dom.setContent("Count", text);
}

function handleCount(dom) {
	var count = dom.itemsLeft();

	if (count != dom.todos.length)
		dom.disableElement("HideClearCompleted",
			() => displayCount(dom, count)
		)
	else
		dom.enableElement("HideClearCompleted",
			() => displayCount(dom, count)
		)
}

function displayTodos(dom) {
	var tree = new Tree();
	var i = 0;

	dom.id = -1;

	tree.pushTag("Todos");

	while (i < dom.todos.length) {
		if (dom.exclude === null || (dom.todos[i]['completed'] != dom.exclude))
			push(dom.todos[i], i, tree);
		i++;
	}

	tree.popTag();

	dom.setLayout("Todos", tree, "Todos.xsl",
		() => dom.focus("Input",
			() => handleCount(dom)
		)
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
	if (dom.id == -1)
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
					} else
						dom.focus("Input");
				}
			)
		);
	else {
		let id = dom.id;
		dom.id = -1;

		dom.getContent("Input." + id,
			(content) => dom.setContent("Input." + id, "",
				() => {
					if (content.trim() != "") {
						dom.todos[id]['label'] = content;
						dom.setContent("Label." + id, content,
							() => dom.removeClasses(
								{
									["View." + id]: "hide",
									["Todo." + id]: "editing"
								}
							),
							() => dom.focus("Input")
						);
					} else {
						dom.todos.splice(id, 1);
						displayTodos(dom);
					}
				}
			)
		);
	}
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
	var i = parseInt(id);

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
	dom.addClass("All", "selected",
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
			() => displayTodos(dom)
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
		if (dom.todos[i]['completed'])
			dom.todos.splice(i, 1);
		else
			i++;
	}

	displayTodos(dom);
}

function acEdit(dom, id) {
	dom.getContent(id,
		(content) => dom.addClasses(
			{
				["View." + content]: "hide",
				[id]: "editing"
			},
			() => {
				dom.id = parseInt(content);
				dom.setContent("Input." + content, dom.todos[dom.id]['label'],
					() => dom.focus("Input." + content))
			}
		)
	);
}

function acCancel(dom, id) {
	var id = dom.id;
	dom.id = -1;
	dom.setContent("Input." + id, "",
		() => dom.removeClasses(
			{
				["View." + id]: "hide",
				["Todo." + id]: "editing"
			},
			() => dom.focus("Input")
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
			"Cancel": acCancel,
		}
	);

	unjsq.launch(newSession, "Connect");
}

main();
