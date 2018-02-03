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
module.exports = njsq;

class Tree {
	constructor() {
		njsq._call(xdhq, 1, this);
	}
	pushTag(name) {
		njsq._call(xdhq, 3, this, name);
	}
	popTag() {
		njsq._call(xdhq, 4, this);
	}
	putValue(value) {
		njsq._call(xdhq, 5, this, value.toString());
	}
	putAttribute(name, value) {
		njsq._call(xdhq, 6, this, name, value.toString());
	}
}

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

class XDH {
	execute(script, callback) {
		njsq._call(xdhq, 9, this, script, callback);
	}
	alert(message, callback) {
		njsq._call(xdhq, 10, this, message, callback);
	}
	confirm(message, callback) {
		njsq._call(xdhq, 11, this, message, (result) => callback(result == "true"));
	}
	setLayout(id, tree, xslFilename, callback) {
		njsq._call(xdhq, 12, this, id, tree, xslFilename, callback);
	}
	getContents(ids, callback) {
		njsq._call(xdhq, 13, this, ids,
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

		njsq._call(xdhq, 14, this, ids, contents, callback);
	}
	setContent(id, content, callback) {
		return this.setContents(merge(id, content), callback);
	}
	dressWidgets(id, callback) {
		njsq._call(xdhq, 15, this, id, callback);
	}
	handleClasses(idsAndClasses, fid, callback) {
		var ids = [];
		var classes = [];

		split(idsAndClasses, ids, classes);

		njsq._call(xdhq, fid, this, ids, classes, callback);
	}
	addClasses(idsAndClasses, callback) {
		this.handleClasses(idsAndClasses, 16, callback);
	}
	addClass(id, clas, callback) {
		this.addClasses(merge(id, clas), callback);
	}
	removeClasses(idsAndClasses, callback) {
		this.handleClasses(idsAndClasses, 17, callback);
	}
	removeClass(id, clas, callback) {
		this.removeClasses(merge(id, clas), callback);
	}
	toggleClasses(idsAndClasses, callback) {
		this.handleClasses(idsAndClasses, 18, callback);
	}
	toggleClass(id, clas, callback) {
		this.toggleClasses(merge(id, clas), callback);
	}
	enableElements(ids, callback) {
		njsq._call(xdhq, 19, this, ids, callback);
	}
	enableElement(id, callback) {
		this.enableElements([id], callback);
	}
	disableElements(ids, callback) {
		njsq._call(xdhq, 20, this, ids, callback);
	}
	disableElement(id, callback) {
		this.disableElements([id], callback);
	}
	setAttribute(id, name, value, callback) {
		njsq._call(xdhq, 21, this, id, name, value, callback);
	}
	getAttribute(id, name, callback) {
		return njsq._call(xdhq, 22, this, id, name, callback);
	}
	removeAttribute(id, name, callback) {
		return njsq._call(xdhq, 23, this, id, name, callback);
	}
	setAttribute(id, name, value, callback) {
		njsq._call(xdhq, 22, this, id, name, value, callback);
	}
	setProperty(id, name, value, callback) {
		njsq._call(xdhq, 24, this, id, name, value, callback );
	}
	getProperty(id, name, callback) {
		return njsq._call(xdhq, 25, this, id, name, callback);
	}
	focus(id, callback) {
		return njsq._call(xdhq, 26, this, id, callback);
	}
}

function register(idsAndItems) {
	var tags = [];
	var callbacks = [];

	split(idsAndItems, tags, callbacks);

	njsq._call(xdhq, 7, tags, callbacks);
}

function launch(callback) {
	njsq._call(xdhq, 8, callback, "53752");
}

module.exports.componentInfo = () => njsq._componentInfo(xdhq);
module.exports.wrapperInfo = () => njsq._wrapperInfo();
module.exports.returnArgument = (text) => { return njsq._call(xdhq, 0, text) };

module.exports.Tree = Tree;
module.exports.register = register;
module.exports.launch = launch;
module.exports.XDH = XDH;
