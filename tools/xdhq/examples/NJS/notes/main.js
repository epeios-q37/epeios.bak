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

var xdhId = "";

if (process.env.EPEIOS_SRC) {
	if (process.platform == 'win32')
		epeiosPath = "h:/hg/epeios/"
	else
		epeiosPath = "~/hg/epeios/"

	xdhId = epeiosPath + "tools/xdhq/wrappers/NJS/XDHq.js";
} else {
	xdhqxdhId = 'xdhq';
}

const xdhq = require(xdhId);

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
		this.notes = [
			// First one must be empty.
			{
				title: '',
				description: '',
			}, {
				title: 'Improve design',
				description: 'Tastes and colors... (aka &laquo;I wish I knew more about CSS...&raquo;)',
			}, {
				title: 'Fixing bugs',
				description: 'There are bugs ? Really ?',
			}, {
				title: 'Implement new functionalities',
				description: "Although it&rsquo;s almost perfect..., isn&rsquo;t it ?",
			}
		];
	}
}

function newSession() {
	LOG();
	console.log("New session detected !");

	return new MyData();
}

function push(note, id, tree) {
	tree.pushTag('Note');
	tree.putAttribute('id', id);
	for (var prop in note) {
		tree.pushTag(prop);
		tree.putValue(note[prop]);
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

	xdh.setCastByTag("Notes", "DescriptionCast", value);
}

function displayList(xdh) {
	LOG();
	var tree = new xdhq.Tree();
	var i = 1 ;
	var contents = new Array();

	tree.pushTag("Notes");
	tree.putAttribute("HideDescriptions", xdh.hideDescriptions);

	while (i < xdh.notes.length) {
		if (xdh.notes[i]['title'].toLowerCase().startsWith(xdh.pattern)) {
			push(xdh.notes[i], i, tree);
			contents.push(["Description." + i, xdh.notes[i]['description']]);
		}
		i++;
	}

	tree.popTag();

	xdh.setLayout("Notes", tree, "Notes.xsl", () => { xdh.setCastByTag("", "EditionCast", "Plain", () => xdh.setContents(contents, () => handleDescriptionsCast(xdh) ) ) } );
}

function acConnect(xdh, id) {
	LOG();

	xdh.setLayout("", new xdhq.Tree(), "Main.xsl", () => { displayList(xdh); });
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
	xdh.setLayout("Edit." + id, new xdhq.Tree(), "Note.xsl",
		() => xdh.setContents(
			[["Title", xdh.notes[xdh.id]['title']],
			["Description", xdh.notes[xdh.id]['description']]],
			() => xdh.setCastsByIds(
				[
					["Edit." + id, "Plain"],
					["View." + id, "Hide"]],
				() => xdh.setCastByTag("", "EditionCast", "Disabled", () => xdh.dressWidgets("Notes" )))));
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
				(result) => { xdh.notes.splice(parseInt(result), 1); displayList(xdh); });
		});
}

function acSubmit(xdh, id) {
	LOG();
	xdh.getContents(["Title", "Description"],
		(result) => {
			var title = result[0].trim();
			var description = result[1];

			if ( title != '') {
				xdh.notes[xdh.id]['title'] = title;
				xdh.notes[xdh.id]['description'] = description;
				if (xdh.id == 0) {
					xdh.notes.unshift({ title: '' , description: '' });
					console.log(">>>>>>> " , xdh.notes);
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
			if ((xdh.notes[xdh.id]['title'] != result[0]) || (xdh.notes[xdh.id]['description'] != result[1]))
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

	xdhq.launch(newSession, "notes", "Connect");
}

main();
