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

/*
	This is the one-file version, which does not need the 'Head.html' and the 'Main.html' files.
	See next comment.
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

/*
	Before publication, remove this comment, all the above lines
	and uncomment following line.
*/
// const atlas = require( 'atlastk' );

const head = [
	'<title>"Hello, World !" example</title>',
	'<style type="text/css">',
	' html, body { height: 100%; padding: 0; margin: 0; }',
	' .vcenter-out, .hcenter { display: table; height: 100%; margin: auto; }',
	' .vcenter-in { display: table-cell; vertical-align: middle; }',
	'</style>',
].join('\n');

const body = [
	'<div class="vcenter-out">',
	' <div class="vcenter-in">',
	'  <fieldset>',
	'   <label>Name:</label>',
	'   <input id="input" maxlength="20" placeholder="Enter a name here"',
	'		type="text" data-xdh-onevent="input|Typing"/>',
	'   <button data-xdh-onevent="Clear">Clear</button>',
	'   <hr/>',
	'   <h1>',
	'    <span>Hello </span>',
	'    <span style="font-style: italic;" id="name"></span>',
	'    <span>!</span>',
	'   </h1>',
	'  </fieldset>',
	' </div>',
	'</div>'].join('\n');

const callbacks = {
	"Connect": (dom, id) => dom.headUp(head, () => dom.setLayout("", body)),
	"Typing": (dom, id) => dom.getContent(id, (name) => dom.setContent("name", name)),
	"Clear": (dom, id) => dom.confirm("Are you sure ?",
		(answer) => { if (answer) dom.setContents({ "input": "", "name": "" }) }),
};

atlas.launch(() => new atlas.DOM(), "Connect", callbacks);