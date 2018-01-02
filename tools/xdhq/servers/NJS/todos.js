/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

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
	along with XDHq. If not, see <http://www.gnu.org/licenses/>.
*/

/* Some logging facilities */

Object.defineProperty(global, '__stack', {
	get: function () {
		var orig = Error.prepareStackTrace;
		Error.prepareStackTrace = function (_, stack) {
			return stack;
		};
		var err = new Error;
		Error.captureStackTrace(err, arguments.callee);
		var stack = err.stack;
		Error.prepareStackTrace = orig;
		return stack;
	}
});

Object.defineProperty(global, '__line', {
	get: function () {
		return __stack[2].getLineNumber();
	}
});

Object.defineProperty(global, '__function', {
	get: function () {
		return __stack[2].getFunctionName();
	}
});

function LOG() {
	console.log("--> " + __function + ": " + __line + " (" + __filename + ")");
}
/**/

const xdhq = require('./XDHq.js');

console.log(xdhq.componentInfo());
console.log(xdhq.wrapperInfo());

// process.stdout.write(xdhq.returnArgument("Text from JS file") + '\n');

class MyData extends xdhq.XDH {
	constructor() {
		super();
		this.timestamp = new Date();
		this.pattern = "";
		this.hideDescriptions = "false";
		this.id = 0;
		this.todos = [
			// First one must be empty.
			{
				title: '',
				description: '',
			}, {
				title: 'Improve design',
				description: "Tastes and colors... (aka 'I wish I knew more about CSS...')",
			}, {
				title: 'Fixing bugs',
				description: 'There are bugs ? Really ?',
			}, {
				title: 'Implement new functionalities',
				description: 'Although it\'s almost perfect...',
			}
		];
	}
}

function newSession() {
	LOG();
	console.log("New session detected !");

	return new MyData();
}

function push(todo, id, tree) {
	tree.pushTag('Todo');
	tree.putAttribute('id', id);
	for (var prop in todo) {
		tree.pushTag(prop);
		tree.putValue(todo[prop]);
		tree.popTag();
	}

	tree.popTag();
}

function handleDescriptionsCast(xdh) {
	LOG();
	var value = "";

	if (xdh.hideDescriptions == "false")
		value = "Plain";
	else
		value = "Hidden";

	xdh.setCastByTag("Todos", "DescriptionCast", value);
}

function displayList(xdh) {
	LOG();
	var tree = new xdhq.LayoutTree();
	var i = 1 ;
	var contents = new Array();

	tree.pushTag("Todos");
	tree.putAttribute("HideDescriptions", xdh.hideDescriptions);

	while (i < xdh.todos.length) {
		if (xdh.todos[i]['title'].toLowerCase().startsWith(xdh.pattern)) {
			push(xdh.todos[i], i, tree);
			contents.push(["Description." + i, xdh.todos[i]['description']]);
		}
		i++;
	}

	tree.popTag();

	xdh.setLayout("Todos", tree, "Todos.xsl", () => { xdh.setCastByTag("", "EditionCast", "Plain", () => xdh.setContents(contents, () => handleDescriptionsCast(xdh) ) ) } );
}

function acConnect(xdh, id) {
	LOG();

	xdh.setLayout("", new xdhq.LayoutTree(), "Main.xsl", () => { displayList(xdh); });
}

function acSearch(xdh, id) {
	LOG();
	xdh.getContent("Pattern", (result) => { xdh.pattern = result.toLowerCase(); displayList(xdh); });
}

function acToggleDescriptions(xdh, id) {
	LOG();
	xdh.getContent(id, (result) => { xdh.hideDescriptions = result; handleDescriptionsCast(xdh); });
}

function view(xdh, id) {
	LOG();
	xdh.setCastsByIds(
		[
			["View." + id, "Plain"],
			["Edit." + id, "Hide"]],
		() => xdh.setCastByTag("", "EditionCast", "Plain",
			() => { xdh.setContent("Edit." + id, ""); xdh.id = -1; }));
}

function edit(xdh, id) {
	LOG();
	console.log(">>>>>>>>> ", id)
	xdh.id = parseInt(id);
	xdh.setLayout("Edit." + id, new xdhq.LayoutTree(), "Todo.xsl",
		() => xdh.setContents(
			[["Title", xdh.todos[xdh.id]['title']],
			["Description", xdh.todos[xdh.id]['description']]],
			() => xdh.setCastsByIds(
				[
					["Edit." + id, "Plain"],
					["View." + id, "Hide"]],
				() => xdh.setCastByTag("", "EditionCast", "Disabled", () => xdh.dressWidgets("Todos" )))));
}

function acEdit(xdh, id) {
	LOG();
	xdh.getContent(id, (result) => edit(xdh, result));
}

function acDelete(xdh, id) {
	LOG();
	xdh.confirm("Are you sure you want to delete this entry ?",
		(response) => {
			if (response) xdh.getContent(id,
				(result) => { xdh.todos.splice(parseInt(result), 1); displayList(xdh); });
		});
}

function acSubmit(xdh, id) {
	LOG();
	xdh.getContents(["Title", "Description"],
		(result) => {
			var title = result[0].trim();
			var description = result[1];

			if ( title != '') {
				xdh.todos[xdh.id]['title'] = title;
				xdh.todos[xdh.id]['description'] = description;
				if (xdh.id == 0) {
					xdh.todos.unshift({ title: '' , description: '' });
					console.log(">>>>>>> " , xdh.todos);
					displayList(xdh);
				} else {
					xdh.setContents(
						[["Title." + xdh.id, title], ["Description." + xdh.id, description]],
					() => view(xdh, xdh.id));
				}
			} else
				xdh.alert("Title can not be empty !");
		});
}

function acCancel(xdh, id) {
	LOG();
	xdh.getContents(["Title", "Description"],
		(result) => {
			if ((xdh.todos[xdh.id]['title'] != result[0]) || (xdh.todos[xdh.id]['description'] != result[1]))
				xdh.confirm("Are you sure you want to cancel your modifications ?",
					(response) => { if (response == true) view(xdh, xdh.id) });
			else
				view(xdh, xdh.id);
		});
}

function main() {
	LOG();
	xdhq.register([
		["Connect", acConnect],
		["ToggleDescriptions", acToggleDescriptions],
		["Search", acSearch],
		["Edit", acEdit],
		["Delete", acDelete],
		["Submit", acSubmit],
		["Cancel", acCancel],
	]);

	xdhq.launch(newSession, "todos", "Connect");
}

main();
