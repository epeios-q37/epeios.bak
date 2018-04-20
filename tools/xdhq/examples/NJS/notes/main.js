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

const viewModeElements = ["Pattern", "CreateButton", "DescriptionToggling", "ViewNotes"];

class MyData extends DOM {
	constructor() {
		super();
		this.timestamp = new Date();
		this.pattern = "";
		this.hideDescriptions = false;
		this.id = 0;
		this.notes = [
			// First one must be empty; it is used as buffer for new entries.
			{
				title: '',
				description: '',
			}, {
				title: 'Improve design',
				description: 'Tastes and colors... (aka &laquo;CSS aren&rsquo;t my cup of tea...&raquo;)',
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

function handleDescriptions(dom) {
	if (dom.hideDescriptions)
		dom.disableElement("ViewDescriptions");
	else
		dom.enableElement("ViewDescriptions");
}

function displayList(dom) {
	var tree = new Tree();
	var i = 1;	// 0 skipped, as it serves as buffer for the new ones.
	var contents = {};

	tree.pushTag("Notes");

	while (i < dom.notes.length) {
		if (dom.notes[i]['title'].toLowerCase().startsWith(dom.pattern)) {
			push(dom.notes[i], i, tree);
			contents["Description." + i] = dom.notes[i]['description'];
		}
		i++;
	}

	tree.popTag();

	dom.setLayout("Notes", tree, "Notes.xsl",
		() => dom.setContents(contents,
			() => dom.enableElements(viewModeElements,
				() => handleDescriptions(dom)
			)
		)
	);
}

function acConnect(dom, id) {
	dom.setLayout("", new Tree(), "Main.xsl",
		() => displayList(dom)
	);
}

function acSearch(dom, id) {
	dom.getContent("Pattern",
		(result) => {
			dom.pattern = result.toLowerCase();
			displayList(dom);
		}
	);
}

function acToggleDescriptions(dom, id) {
	dom.getContent(id,
		(result) => {
			dom.hideDescriptions = result == "true";
			handleDescriptions(dom);
		}
	);
}

function view(dom) {
	dom.enableElements(
		viewModeElements,
		() => {
			dom.setContent("Edit." + dom.id, "");
			dom.id = -1;
		}
	);
}

function edit(dom, id) {
	dom.id = parseInt(id);
	dom.setLayout("Edit." + id, new Tree(), "Note.xsl",
		() => dom.setContents(
			{
				"Title": dom.notes[dom.id]['title'],
				"Description": dom.notes[dom.id]['description'],
			},
			() => dom.disableElements(
				viewModeElements,
				() => dom.dressWidgets("Notes")
			)
		)
	);
}

function acEdit(dom, id) {
	dom.getContent(id,
		(result) => edit(dom, result)
	);
}

function acDelete(dom, id) {
	dom.confirm("Are you sure you want to delete this entry ?",
		(response) => {
			if (response) dom.getContent(id,
				(result) => {
					dom.notes.splice(parseInt(result), 1);
					displayList(dom);
				}
			);
		}
	);
}

function acSubmit(dom, id) {
	dom.getContents(["Title", "Description"],
		(result) => {
			var title = result['Title'].trim();
			var description = result['Description'];

			if (title != '') {
				dom.notes[dom.id]['title'] = title;
				dom.notes[dom.id]['description'] = description;
				if (dom.id == 0) {
					dom.notes.unshift({ title: '', description: '' });
					displayList(dom);
				} else {
					let contents = {};
					contents["Title." + dom.id] = title;
					contents["Description." + dom.id] = description;
					dom.setContents(contents,
						() => view(dom)
					);
				}
			} else
				dom.alert("Title can not be empty !");
		}
	);
}

function acCancel(dom, id) {
	dom.getContents(["Title", "Description"],
		(result) => {
			if ((dom.notes[dom.id]['title'] != result['Title']) || (dom.notes[dom.id]['description'] != result['Description']))
				dom.confirm("Are you sure you want to cancel your modifications ?",
					(response) => {
						if (response == true) view(dom);
					}
				);
			else
				view(dom);
		}
	);
}

function main() {
	atlas.register(
		{
			"Connect": acConnect,
			"ToggleDescriptions": acToggleDescriptions,
			"Search": acSearch,
			"Edit": acEdit,
			"Delete": acDelete,
			"Submit": acSubmit,
			"Cancel": acCancel,
		}
	);

	atlas.launch(newSession, "Connect" );
}

main();