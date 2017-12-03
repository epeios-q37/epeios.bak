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

class XDH {
	setLayout(id, xml, xslFilename) {
		njsq._wrapper( 9, this, id, xml, xslFilename);
	}
}



module.exports.returnArgument = (text) => { return njsq._wrapper(0, text); };
module.exports.Tree = Tree;
module.exports.register = ( tag, callback ) => { njsq._wrapper( 7, tag, callback ); };
module.exports.listen = (callback, args) => { njsq._wrapper(8, callback, args); };
module.exports.XDH = XDH;

