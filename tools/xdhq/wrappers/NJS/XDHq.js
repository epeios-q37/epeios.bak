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
		xslPath = path.join( epeiosToolsPath, "xdhq/servers/" );
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
		njsq._wrapper(xdhq, 1, this );
	}
	pushTag(name) {
		njsq._wrapper(xdhq, 3, this, name);
	}
	popTag() {
		njsq._wrapper(xdhq, 4, this);
	}
	putValue( value ) {
		njsq._wrapper(xdhq, 5, this, value.toString());
	}
	putAttribute(name, value) {
		njsq._wrapper(xdhq, 6, this, name, value.toString());
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

function nop() {
}

function normalize(callback) {
	if (typeof callback === "undefined")
		return nop;
	else
		return callback;
}

class XDH {
	alert(message, callback) {
		njsq._wrapper(xdhq, 9, this, message, normalize(callback));
	}
	confirm(message, callback) {
		njsq._wrapper(xdhq, 10, this, message, (result) => normalize(callback)( result == "true" ));
	}
	setLayout(id, tree, xslFilename, callback) {
		njsq._wrapper(xdhq, 11, this, id, tree, xslFilename, normalize(callback));
	}
	getContents(ids, callback )  {
		njsq._wrapper(xdhq, 12, this, ids, normalize( callback) );
	}
	getContent(id, callback ) {
		return this.getContents([id], (result) => { normalize(callback)(result[0]); } );
	}
	setContents(idsAndContents, callback) {
		var ids = new Array();
		var contents = new Array();

		pushLabelsAndItems(idsAndContents, "string", ids, contents);

		njsq._wrapper(xdhq, 13, this, ids, contents, normalize(callback));
	}
	setContent(id, content, callback) {
		return this.setContents([[id,content]], normalize(callback));
	}
	dressWidgets(id, callback) {
		njsq._wrapper(xdhq, 14, this, id, normalize(callback));
	}
	setCastsByIds(idsAndValues, callback ) {
		var ids = new Array();
		var values = new Array();

		pushLabelsAndItems(idsAndValues, "string", ids, values);

		njsq._wrapper(xdhq, 15, this, ids, values, normalize(callback));
	}
	setCastById(id, value, callback ) {
		this.setCastsByIds([[id, value]], normalize(callback) );
	}
	setCastsByTags(id, tagsAndValues, callback) {
		var tags = new Array();
		var values = new Array();

		pushLabelsAndItems(tagsAndValues, "string", tags, values);

		njsq._wrapper(xdhq, 16, this, id, tags, values, normalize(callback));
	}
	setCastByTag(id, tag, value, callback) {
		this.setCastsByTags(id, [[tag, value]], normalize(callback));
	}
	getAttribute(id, name, callback) {
		return njsq._wrapper(xdhq, 17, this, id, name, normalize(callback));
	}
	setAttribute(id, name, value, callback ) {
		njsq._wrapper(xdhq, 18, this, id, name, value, normalize(callback));
	}
	getProperty(id, name) {
		return njsq._wrapper(xdhq, 19, this, id, name);
	}
	setProperty(id, name, value) {
		njsq._wrapper(xdhq, 20, this, id, name, value);
	}
}

function register(idsAndItems) {
	var tags = new Array();
	var callbacks = new Array();

	pushLabelsAndItems(idsAndItems, "function", tags, callbacks);

	njsq._wrapper(xdhq, 7, tags, callbacks);
}

function launch( callback ) {
	njsq._wrapper(xdhq, 8, callback, "53752" );
}

module.exports.componentInfo = () => njsq._componentInfo(xdhq);
module.exports.wrapperInfo = () => njsq._wrapperInfo();
module.exports.returnArgument = (text) => { return njsq._wrapper(xdhq, 0, text) };

module.exports.Tree = Tree;
module.exports.register = register;
module.exports.launch = launch;
module.exports.XDH = XDH;

