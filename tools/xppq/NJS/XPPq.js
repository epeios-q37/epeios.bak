/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
*/

"use strict"

var affix = "xppq";

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

module.exports.returnArgument = (text) => { return njsq._wrapper( 0, text ) };

const stream = require('stream');

class Stream extends stream.Readable {
    constructor(stream, options) {
        super(options);
        stream.on('readable', () => { var chunk = stream.read(); if (chunk == null) njsq._wrapper(5, stream); else njsq._wrapper(4, stream, chunk); });
        njsq._wrapper( 7, stream, this );
    }
    _read(size) {
        njsq._wrapper( 6, this );
    }
}


// If modified, modify also 'parser.cpp'.
var tokens = {
    ERROR: 0,
    START_TAG: 1,
    ATTRIBUTE: 2,
    VALUE: 3,
    END_TAG: 4
};

module.exports = njsq;
module.exports.Stream = Stream;
module.exports.parse = (stream, callback) => { stream.on('readable', () => { var chunk = stream.read(); if ( chunk == null ) njsq._wrapper( 2, stream ); else njsq._wrapper(1, stream, chunk); } ); njsq._wrapper(3, stream, callback) };
module.exports.tokens = tokens;
