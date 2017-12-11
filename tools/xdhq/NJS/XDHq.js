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

// Begin of generic part.
var njsq = null;
var componentPath = null;
var componentFilename = null;
var path = require("path");

if (process.env.EPEIOS_SRC) {
    if (process.platform == 'win32')
        componentPath = 'h:/bin/';
    else
        componentPath = '~/bin/';
    njsq = require(componentPath + 'njsq.node');
} else {
    njsq = require('njsq');
    componentPath = __dirname;
}

componentFilename = path.join(componentPath, affix + "njs").replace(/\\/g, "\\\\").replace(/'/g, "\\'").replace(/ /g, "\\ ");
njsq.register(componentFilename);
module.exports = njsq;
// End of generic part.

class Tree {
	constructor( background ) {
		njsq._wrapper( 1, this, background );
	}
	pushTag(name) {
		njsq._wrapper(3, this, name);
	}
	popTag() {
		njsq._wrapper(4);
	}
	putValue( value ) {
		njsq._wrapper(5, this);
	}
	putAttribute(name, value) {
		njsq._wrapper(6, this, name, value);
	}
}

class LayoutTree extends Tree {
	constructor() {
		super("Layout");
	}
}

class CastingTree extends Tree {
	constructor() {
		super("Casting");
	}
}

function pushLabelAndItem(labelAndItem, itemType, labels, items) {
	if ((labelAndItem instanceof Array) && (labelAndItem.length === 2) && (typeof labelAndItem[0] === "string") && (typeof labelAndItem[1] === itemType)) {
		labels.push(labelAndItem[0]);
		items.push(labelAndItem[1]);
	} else
		throw ("Error in parameters.");
}

function pushLabelsAndItems(labelsAndItems, itemType, labels, items) {
	var length = labelsAndItems.length;

	while (length--) {
		pushLabelAndItem(labelsAndItems[length], itemType, labels, items);
	}
}

// '[a,b],[c,d],[e,f]' => '[a,c,e],[b,d,f]'
function normalize(labelOrLabelsAndItems, item, itemType, labels, items) {
	if ((typeof labelOrLabelsAndItems === "string") && (typeof item === itemType)) {
		labels.push(labelOrLabelsAndItems);
		items.push(item);
	} else if ((labelOrLabelsAndItems instanceof Array) && (typeof item === "undefined")) {
		pushLabelsAndItems(labelOrLabelsAndItems, itemType, labels, items);	// Mixed array not implemented yet.
	} else
		throw ("Error in parameters.");
}

class XDH {
	set(fid, tree, xslFilename, id) {
		if ( !id )
			id = "Root";

		njsq._wrapper(fid, this, id, tree, xslFilename);
	}
	setLayout(tree, xslFilename, id) {
		this.set(9, tree, xslFilename, id);
	}
	getContent(id) {
		return njsq._wrapper(10, this, id);
	}
	setContent(idOrIdsAndContents, content) {
		var ids = new Array();
		var contents = new Array();

		normalize(idOrIdsAndContents, content, "string", ids, contents);

		return njsq._wrapper(11, this, ids, contents);
	}
	setCasts(id, tagOrTagsAndValues, value) {
		var tags = new Array();
		var values = new Array();

		normalize(tagOrTagsAndValues, value, "string", tags, values);

		njsq._wrapper(12, this, id, tags, values);
	}
}

function register(idOrIdsAndItems, item) {
	var tags = new Array();
	var callbacks = new Array();

	normalize(idOrIdsAndItems, item, "function", tags, callbacks);

	njsq._wrapper(7, tags, callbacks);
}

module.exports.returnArgument = (text) => { return njsq._wrapper(0, text); };
module.exports.LayoutTree = LayoutTree;
module.exports.CastingTree = CastingTree;
module.exports.register = register;
module.exports.listen = (callback, args) => { njsq._wrapper(8, callback, args); };
module.exports.XDH = XDH;

