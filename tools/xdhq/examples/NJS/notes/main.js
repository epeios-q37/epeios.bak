/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

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

const viewModeElements = ["Pattern", "CreateButton", "DescriptionToggling", "ViewButtonsOnHover"];

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
	if (dom.hideDescriptions )
		dom.disableElement("ViewDescriptions");
	else
		dom.enableElement("ViewDescriptions");
}

function displayList(dom) {
	var tree = new Tree();
	var i = 1;	// 0 skipped, as it serves as buffer for the new ones.
	var contents = new Array();

	tree.pushTag("Notes");
	tree.putAttribute("HideDescriptions", dom.hideDescriptions );

	while (i < dom.notes.length) {
		if (dom.notes[i]['title'].toLowerCase().startsWith(dom.pattern)) {
			push(dom.notes[i], i, tree);
			contents.push(["Description." + i, dom.notes[i]['description']]);
		}
		i++;
	}

	tree.popTag();

	dom.setLayout("Notes", tree, "Notes.xsl",
		() => dom.setContents(contents,
			() => dom.enableElements( viewModeElements,
				() => handleDescriptions(dom )
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

function view(dom, id) {
	dom.enableElements(
		viewModeElements,
			() => {
				dom.setContent("Edit." + id, "");
				dom.id = -1;
			}
	);
}

function edit(dom, id) {
	dom.id = parseInt(id);
	dom.setLayout("Edit." + id, new Tree(), "Note.xsl",
		() => dom.setContents(
			[
				["Title", dom.notes[dom.id]['title']],
				["Description", dom.notes[dom.id]['description']]
			],
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
			var title = result[0].trim();
			var description = result[1];

			if (title != '') {
				dom.notes[dom.id]['title'] = title;
				dom.notes[dom.id]['description'] = description;
				if (dom.id == 0) {
					dom.notes.unshift({ title: '', description: '' });
					displayList(dom);
				} else {
					dom.setContents(
						[
							["Title." + dom.id, title],
							["Description." + dom.id, description]
						],
						() => view(dom, dom.id)
					);
				}
			} else
				dom.alert("Title can not be empty !");
		});
}

function acCancel(dom, id) {
	dom.getContents(["Title", "Description"],
		(result) => {
			if ((dom.notes[dom.id]['title'] != result[0]) || (dom.notes[dom.id]['description'] != result[1]))
				dom.confirm("Are you sure you want to cancel your modifications ?",
					(response) => {
						if (response == true) view(dom, dom.id);
					}
				);
			else
				view(dom, dom.id);
		});
}

function main() {
	unjsq.register([
		["Connect", acConnect],
		["ToggleDescriptions", acToggleDescriptions],
		["Search", acSearch],
		["Edit", acEdit],
		["Delete", acDelete],
		["Submit", acSubmit],
		["Cancel", acCancel],
	]);

	unjsq.launch(newSession, "Connect");
}

main();
