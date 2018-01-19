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
	along with UnJSq. If not, see <http://www.gnu.org/licenses/>.
*/

"use strict"

const path = require( 'path' );

var xdhqId = "";
var xdhwebqId = "";

function isDev() {
	if (process.env.EPEIOS_SRC)
		return true;
	else
		return false;
}

function getEpeiosPath() {
	if ( isDev ) {
		if (process.platform == 'win32') {
			return "h:/hg/epeios/"
		} else {
			return "~/hg/epeios/"
		}
	} else
		throw "Error !";
}

function getRealDir( dir ) {
	if ( isDev() ) {
		let epeiosPath = getEpeiosPath();
		return path.resolve(epeiosPath, "tools/xdhq/examples/common/", path.relative(path.resolve(epeiosPath, "tools/xdhq/examples/NJS/"), path.resolve(dir)));	// No final '/'.
	} else
		return path.resolve( dir );
}

if (isDev()) {
	let epeiosToolsPath = "";
	if (process.platform == 'win32')
		epeiosToolsPath = "h:/hg/epeios/tools/";
	else
		epeiosToolsPath = "~/hg/epeios/tools/";

	xdhwebqId = epeiosToolsPath + "xdhwebq/NJS/XDHWebQ.js";
	xdhqId = epeiosToolsPath + "xdhq/wrappers/NJS/XDHq.js";
} else {
	xdhwebqId = "xdhwebq";
	xdhqId = "xdhq";
}

const xdhwebq = require(xdhwebqId);
const xdhq = require(xdhqId);

function launch(callback, action) {
	xdhq.launch(callback);
	require('child_process').fork(require(xdhwebqId).fileName, [getRealDir(path.dirname(process.argv[1])), action]);
}

module.exports.register = xdhq.register;
module.exports.launch = launch;
module.exports.Tree = xdhq.Tree;
module.exports.DOM = xdhq.XDH;
