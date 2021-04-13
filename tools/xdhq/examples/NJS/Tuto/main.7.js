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
		this.exclude = null;
		this.index = -1;
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

function newSession() {
	console.log("New session detected !");

	return new MyData();
}

function acConnect(dom, id) {
	dom.inner("", new Tree(), "Main.7.xsl");
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

	dom.setValue("Count", text);
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

	dom.inner("Todos", tree, "Todos.4.xsl",
		() => handleCount(dom)
	);
}

function acSubmit(dom, id) {
	dom.getValue("Input",
		(content) => dom.setValue("Input", "",
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
	dom.getValue(id,
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
		() => handleCount(dom)
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
		}
	);

	atlas.launch(newSession, "Connect");
}

main();
