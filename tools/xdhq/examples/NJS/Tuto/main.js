/*
MIT License

Copyright (c) 2018 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
This is the Node.js Atlas (http://atlastk/) version
of the TodoMVC application (http://todomvc.com/).
*/

"use strict"

var atlasId = "";

if (process.env.Q37_EPEIOS) {
	let epeiosPath = "";

	if (process.platform == 'win32')
		epeiosPath = "h:/hg/epeios/"
	else
		epeiosPath = process.env.Q37_EPEIOS

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
		this.timestamp = new Date();
		this.exclude = null;
		this.index = -1;
		this.todos = [
			{
				"completed": true,
				"label": "Note 1"
			}, {
				"completed": false,
				"label": "Note 2"
			}
		];
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
			text = count + " items left";
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

	dom.index = -1;

	tree.pushTag("Todos");

	while (i < dom.todos.length) {
		if (dom.exclude === null || (dom.todos[i]['completed'] != dom.exclude))
			push(dom.todos[i], i, tree);
		i++;
	}

	tree.popTag();

	dom.inner("Todos", tree, "Todos.xsl",
		() => handleCount(dom)
	);
}

function newSession() {
	console.log("New session detected !");

	return new MyData();
}

function acConnect(dom, id) {
	dom.inner("", new Tree(), "Main.xsl",
		() => dom.focus("Input",
			() => displayTodos(dom)
		)
	);
}

function submitNew(dom) {
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

function submitModification(dom) {
	let id = dom.index;
	dom.index = -1;

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
						)
					);
				} else {
					dom.todos.splice(id, 1);
					displayTodos(dom);
				}
			}
		)
	);
}

function acSubmit(dom, id) {
	if (dom.index == -1) {
		submitNew(dom);
	} else {
		submitModification(dom);
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
				"Completed": "selected"
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
				"Completed": "selected"
			},
			() => displayTodos(dom)
		)
	)
}

function acCompleted(dom, id) {
	dom.exclude = false;

	dom.addClass("Completed", "selected",
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
				dom.index = parseInt(content);
				dom.setContent("Input." + content, dom.todos[dom.index]['label'],
					() => dom.focus("Input." + content))
			}
		)
	);
}

function acCancel(dom, id) {
	var id = dom.index;
	dom.index = -1;

	dom.setContent("Input." + id, "",
		() => dom.removeClasses(
			{
				["View." + id]: "hide",
				["Todo." + id]: "editing"
			}
		)
	);
}

function main() {
	atlas.register(
		{
			"Connect": acConnect,
			"Submit": acSubmit,
			"Destroy": acDestroy,
			"Toggle": acToggle,
			"All": acAll,
			"Active": acActive,
			"Completed": acCompleted,
			"Clear": acClear,
			"Edit": acEdit,
			"Cancel": acCancel,
		}
	);

	atlas.launch(newSession, "Connect");
}

main();
