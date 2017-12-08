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

class XDH {
	set(fid, tree, xslFilename, id) {
		if ( !id )
			id = "Root";

		njsq._wrapper(fid, this, id, tree, xslFilename);
	}
	setLayout(tree, xslFilename, id) {
		this.set(9, tree, xslFilename, id);
	}
	setCasting(tree, xslFilename, id) {
		this.set(10, tree, xslFilename, id);
	}
	getContent(id) {
		return njsq._wrapper(11, this, id);
	}
	setContent(id, content) {
		return njsq._wrapper(12, this, id, content);
	}
}

function isFunction(functionToCheck) {
	var getType = {};
	return functionToCheck && getType.toString.call(functionToCheck) === '[object Function]';
}

function pushIdAndItem( idAndItem, itemType, ids, items ) {
	if ( ( idAndItem instanceof Array ) && ( idAndItem.length === 2 ) && ( typeof idAndItem[0] === "string" ) && ( typeof idAndItem[1] === itemType ) ) {
		ids.push( idAndItem[0] );
		items.push( idAndItem[1] );
	} else
		throw( "Error in parameters.");
}

function pushIdsAndItems( idsAndItems, itemType, ids, items )
{
	var length = idsAndItems.length;

	while (length--) {
		pushIdAndItem( idsAndItems[length], itemType, ids , items );
	}
}

function callWrapperWithIdsAndItems( fid, itemType, idOrIdsAndItems, item )
{
	var ids = new Array();
	var items = new Array();

	if ((typeof idOrIdsAndItems === "string") && (typeof item === itemType)) {
		ids.push(idOrIdsAndItems);
		items.push( item );
	} else if ((idOrIdsAndItems instanceof Array) && (typeof item === "undefined")) {
		pushIdsAndItems(idOrIdsAndItems, itemType, ids, items);	// Mixed array not implemented yet.
	} else
		throw( "Error in parameters.");

	njsq._wrapper(fid, ids, items);
}

module.exports.returnArgument = (text) => { return njsq._wrapper(0, text); };
module.exports.LayoutTree = LayoutTree;
module.exports.CastingTree = CastingTree;
module.exports.register = (idOrIdsAndItems, item) => { callWrapperWithIdsAndItems(7, "function", idOrIdsAndItems, item); };
module.exports.listen = (callback, args) => { njsq._wrapper(8, callback, args); };
module.exports.XDH = XDH;

