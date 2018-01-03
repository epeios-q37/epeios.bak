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

// process.stdout.write(xdhq.returnArgument("Text from JS file") + '\n');

class MyData extends UnJS {
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

function handleDescriptionsCast(unjs) {
	LOG();
	var value = "";

	if (unjs.hideDescriptions == "false")
		value = "Plain";
	else
		value = "Hidden";

	unjs.setCastByTag("Notes", "DescriptionCast", value);
}

function displayList(unjs) {
	LOG();
	var tree = new Tree();
	var i = 1 ;
	var contents = new Array();

	tree.pushTag("Notes");
	tree.putAttribute("HideDescriptions", unjs.hideDescriptions);

	while (i < unjs.notes.length) {
		if (unjs.notes[i]['title'].toLowerCase().startsWith(unjs.pattern)) {
			push(unjs.notes[i], i, tree);
			contents.push(["Description." + i, unjs.notes[i]['description']]);
		}
		i++;
	}

	tree.popTag();

	unjs.setLayout("Notes", tree, "Notes.xsl", () => { unjs.setCastByTag("", "EditionCast", "Plain", () => unjs.setContents(contents, () => handleDescriptionsCast(unjs))) });
}

function acConnect(unjs, id) {
	LOG();

	unjs.setLayout("", new Tree(), "Main.xsl", () => { displayList(unjs); });
}

function acSearch(unjs, id) {
	LOG();
	unjs.getContent("Pattern", (result) => { unjs.pattern = result.toLowerCase(); displayList(unjs); });
}

function acToggleDescriptions(unjs, id) {
	LOG();
	unjs.getContent(id, (result) => { unjs.hideDescriptions = result; handleDescriptionsCast(unjs); });
}

function view(unjs, id) {
	LOG();
	unjs.setCastsByIds(
		[
			["View." + id, "Plain"],
			["Edit." + id, "Hide"]],
		() => unjs.setCastByTag("", "EditionCast", "Plain",
			() => { unjs.setContent("Edit." + id, ""); unjs.id = -1; }));
}

function edit(unjs, id) {
	LOG();
	unjs.id = parseInt(id);
	unjs.setLayout("Edit." + id, new Tree(), "Note.xsl",
		() => unjs.setContents(
			[["Title", unjs.notes[unjs.id]['title']],
			["Description", unjs.notes[unjs.id]['description']]],
			() => unjs.setCastsByIds(
				[
					["Edit." + id, "Plain"],
					["View." + id, "Hide"]],
				() => unjs.setCastByTag("", "EditionCast", "Disabled", () => unjs.dressWidgets("Notes")))));
}

function acEdit(unjs, id) {
	LOG();
	unjs.getContent(id, (result) => edit(unjs, result));
}

function acDelete(unjs, id) {
	LOG();
	unjs.confirm("Are you sure you want to delete this entry ?",
		(response) => {
			if (response) unjs.getContent(id,
				(result) => { unjs.notes.splice(parseInt(result), 1); displayList(unjs); });
		});
}

function acSubmit(unjs, id) {
	LOG();
	unjs.getContents(["Title", "Description"],
		(result) => {
			var title = result[0].trim();
			var description = result[1];

			if ( title != '') {
				unjs.notes[unjs.id]['title'] = title;
				unjs.notes[unjs.id]['description'] = description;
				if (unjs.id == 0) {
					unjs.notes.unshift({ title: '', description: '' });
					displayList(unjs);
				} else {
					unjs.setContents(
						[["Title." + unjs.id, title], ["Description." + unjs.id, description]],
					() => view(unjs, unjs.id));
				}
			} else
				unjs.alert("Title can not be empty !");
		});
}

function acCancel(unjs, id) {
	LOG();
	unjs.getContents(["Title", "Description"],
		(result) => {
			if ((unjs.notes[unjs.id]['title'] != result[0]) || (unjs.notes[unjs.id]['description'] != result[1]))
				unjs.confirm("Are you sure you want to cancel your modifications ?",
					(response) => { if (response == true) view(unjs, unjs.id) });
			else
				view(unjs, unjs.id);
		});
}

function main() {
	LOG();
	unjs.register([
		["Connect", acConnect],
		["ToggleDescriptions", acToggleDescriptions],
		["Search", acSearch],
		["Edit", acEdit],
		["Delete", acDelete],
		["Submit", acSubmit],
		["Cancel", acCancel],
	]);

	unjs.launch(newSession, "notes", "Connect");
}

main();
