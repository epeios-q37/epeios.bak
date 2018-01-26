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

function pushLabelAndItem(labelAndItem, itemType, labels, items) {
	if ((labelAndItem instanceof Array) && (labelAndItem.length === 2) && (typeof labelAndItem[0] === "string") && (typeof labelAndItem[1] === itemType)) {
		labels.push(labelAndItem[0]);
		items.push(labelAndItem[1]);
	} else
		throw ("Error in parameters.");
}

// '[a,b],[c,d],[e,f]' => '[a,c,e],[b,d,f]'
function pushLabelsAndItems(labelsAndItems, itemType, labels, items) {
	var length = labelsAndItems.length;

	while (length--) {
		pushLabelAndItem(labelsAndItems[length], itemType, labels, items);
	}
}

class XDH {
	alert(message, callback) {
		njsq._call(xdhq, 9, this, message, callback);
	}
	confirm(message, callback) {
		njsq._call(xdhq, 10, this, message, (result) => callback(result == "true"));
	}
	setLayout(id, tree, xslFilename, callback) {
		njsq._call(xdhq, 11, this, id, tree, xslFilename, callback);
	}
	getContents(ids, callback) {
		njsq._call(xdhq, 12, this, ids, callback);
	}
	getContent(id, callback) {
		return this.getContents([id], (result) => { callback(result[0]); });
	}
	setContents(idsAndContents, callback) {
		var ids = new Array();
		var contents = new Array();

		pushLabelsAndItems(idsAndContents, "string", ids, contents);

		njsq._call(xdhq, 13, this, ids, contents, callback);
	}
	setContent(id, content, callback) {
		return this.setContents([[id, content]], callback);
	}
	dressWidgets(id, callback) {
		njsq._call(xdhq, 14, this, id, callback);
	}
	handleClasses(idsAndClasses, fid, callback) {
		var ids = new Array();
		var classes = new Array();

		pushLabelsAndItems(idsAndClasses, "string", ids, classes);

		njsq._call(xdhq, fid, this, ids, classes, callback);
	}
	addClasses(idsAndClasses, callback) {
		this.handleClasses(idsAndClasses, 15, callback);
	}
	addClass(id, clas, callback) {
		this.addClasses([[id, clas]], callback);
	}
	removeClasses(idsAndClasses, callback) {
		this.handleClasses(idsAndClasses, 16, callback);
	}
	removeClass(id, clas, callback) {
		this.removeClasses([[id, clas]], callback);
	}
	toggleClasses(idsAndClasses, callback) {
		this.handleClasses(idsAndClasses, 17, callback);
	}
	toggleClass(id, clas, callback) {
		this.toggleClasses([[id, clas]], callback);
	}
	enableElements(ids, callback) {
		njsq._call(xdhq, 18, this, ids, callback);
	}
	enableElement(id, callback) {
		this.enableElements([id], callback);
	}
	disableElements(ids, callback) {
		njsq._call(xdhq, 19, this, ids, callback);
	}
	disableElement(id, callback) {
		this.disableElements([id], callback);
	}
	getAttribute(id, name, callback) {
		return njsq._call(xdhq, 20, this, id, name, callback);
	}
	setAttribute(id, name, value, callback) {
		njsq._call(xdhq, 21, this, id, name, value, callback);
	}
	getProperty(id, name) {
		return njsq._call(xdhq, 22, this, id, name);
	}
	setProperty(id, name, value) {
		njsq._call(xdhq, 23, this, id, name, value);
	}
}

function register(idsAndItems) {
	var tags = new Array();
	var callbacks = new Array();

	pushLabelsAndItems(idsAndItems, "function", tags, callbacks);

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

