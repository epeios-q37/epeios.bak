/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

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

"use strict"

var affix = "xdhq";

var njsq = null;
var componentPath = null;
var componentFilename = null;
var path = require("path");
var xslPath = "./";
var epeiosToolsPath = "";

if (process.env.EPEIOS_SRC) {
	if (process.platform == 'win32') {
		componentPath = 'h:/bin/';
		epeiosToolsPath = "h:/hg/epeios/tools/";
		xslPath = path.join(epeiosToolsPath, "xdhq/servers/");
	} else {
		componentPath = '~/bin/';
		epeiosPath = "~/hg/epeios/tools/";
		xslPath = path.join(epeiosToolsPath, "xdhq/servers/");
	}
	njsq = require(componentPath + 'njsq.node');
} else {
	njsq = require('njsq');
	componentPath = __dirname;
}

componentFilename = path.join(componentPath, affix + "njs").replace(/\\/g, "\\\\").replace(/'/g, "\\'").replace(/ /g, "\\ ");
const xdhq = njsq._register(componentFilename);
// module.exports = njsq;


// {'a': b, 'c': d, 'e': f} -> ['a','c','e'] [b,d,f]
function split(keysAndValues, keys, values) {
	for (var prop in keysAndValues) {
		keys.push(prop);
		values.push(keysAndValues[prop]);
	}
}

// ['a', 'b', 'c'] ['d', 'e', 'f'] -> { 'a': 'd', 'b': 'e', 'c': 'f' }
function unsplit(keys, values) {
	var i = 0;
	var keysValues = {};

	while (i < keys.length) {
		keysValues[keys[i]] = values[i];
		i++;
	}

	return keysValues;
}

// 'key', value -> { 'key': value } 
function merge(key, value) {
	var keyValue = {};

	keyValue[key] = value;

	return keyValue;
}

const types = {
	VOID: 0,
	STRING: 1,
	STRINGS: 2
}

class XDH {
	execute(script, callback) {
		//		njsq._call(xdhq, 9, this, script, callback);
		njsq._call(xdhq, 3, this, "Execute_1", types.VOID, 1, script, 0, callback);
	}
	alert(message, callback) {
		njsq._call(xdhq, 3, this, "", types.VOID, 1, message, 0, callback);
	}
	confirm(message, callback) {
		njsq._call(xdhq, 3, this, "Confirm_1", types.STRING, 1, message, 0, (result) => callback(result == "true"));
	}
	setLayout(id, tree, xslFilename, callback) {
		njsq._call(xdhq, 3, this, "SetLayout_1", types.VOID, 3, id, tree.end(), xslFilename, 0, callback);
	}
	getContents(ids, callback) {
		njsq._call(xdhq, 3, this, "GetContents_1", types.STRINGS, 0, 1, ids,
			(contents) => callback(unsplit(ids, contents))
		);
	}
	getContent(id, callback) {
		return this.getContents([id], (result) => { callback(result[id]); });
	}
	setContents(idsAndContents, callback) {
		var ids = [];
		var contents = [];

		split(idsAndContents, ids, contents);

		njsq._call(xdhq, 3, this, "SetContents_1", types.VOID, 0, 2, ids, contents, callback);
	}
	setContent(id, content, callback) {
		return this.setContents(merge(id, content), callback);
	}
	dressWidgets(id, callback) {
		njsq._call(xdhq, 3, this, "DressWidget_1", types.VOID, 1, id, 0, callback);
	}
	handleClasses(idsAndClasses, command, callback) {
		var ids = [];
		var classes = [];

		split(idsAndClasses, ids, classes);

		njsq._call(xdhq, 3, this, command, types.VOID, 0, 2, ids, classes, callback);
	}
	addClasses(idsAndClasses, callback) {
		this.handleClasses(idsAndClasses, "AddClasses_1", callback);
	}
	addClass(id, clas, callback) {
		this.addClasses(merge(id, clas), callback);
	}
	removeClasses(idsAndClasses, callback) {
		this.handleClasses(idsAndClasses, "RemoveClasses_1", callback);
	}
	removeClass(id, clas, callback) {
		this.removeClasses(merge(id, clas), callback);
	}
	toggleClasses(idsAndClasses, callback) {
		this.handleClasses(idsAndClasses, "ToggleClasses_1", callback);
	}
	toggleClass(id, clas, callback) {
		this.toggleClasses(merge(id, clas), callback);
	}
	enableElements(ids, callback) {
		njsq._call(xdhq, 3, this, "EnableElements_1", types.VOID, 0, 1, ids, callback);
	}
	enableElement(id, callback) {
		this.enableElements([id], callback);
	}
	disableElements(ids, callback) {
		njsq._call(xdhq, 3, this, "DisableElements_1", types.VOID, 0, 1, ids, callback);
	}
	disableElement(id, callback) {
		this.disableElements([id], callback);
	}
	setAttribute(id, name, value, callback) {
		njsq._call(xdhq, 3, this, "SetAttribute_1", types.VOID, 3, id, name, value, 0, callback);
	}
	getAttribute(id, name, callback) {
		return njsq._call(xdhq, 3, this, "GetAttribute_1", types.STRING, 2, id, name, 0, callback);
	}
	removeAttribute(id, name, callback) {
		njsq._call(xdhq, 3, this, "RemoveAttribute_1", types.VOID, 2, id, name, 0, callback);
	}
	setProperty(id, name, value, callback) {
		njsq._call(xdhq, 3, this, "SetProperty_1", types.VOID, 3, id, name, value, 0, callback);
	}
	getProperty(id, name, callback) {
		return njsq._call(xdhq, 3, this, "GetProperty_1", types.STRING, 2, id, name, 0, callback);
	}
	focus(id, callback) {
		njsq._call(xdhq, 3, this, "Focus_1", types.VOID, 1, id, 0, callback);
	}
}

function register(idsAndItems) {
	var tags = [];
	var callbacks = [];

	split(idsAndItems, tags, callbacks);

	njsq._call(xdhq, 1, tags, callbacks);
}

function launch(callback, action) {
	njsq._call(xdhq, 2, callback, "53752", action);
}

module.exports.componentInfo = () => njsq._componentInfo(xdhq);
module.exports.wrapperInfo = () => njsq._wrapperInfo();
module.exports.returnArgument = (text) => { return njsq._call(xdhq, 0, text) };

module.exports.createTree = () => require('xmlbuilder').create('XDHTML');
module.exports.register = register;
module.exports.launch = launch;
module.exports.XDH = XDH;
