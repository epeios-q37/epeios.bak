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
const xppq = njsq._register(componentFilename);
// End of generic part.

module.exports.returnArgument = (text) => { return njsq._wrapper( xppq, 0, text ) };

const stream = require('stream');

function onReadable(stream, onData, onEnd) {
    var chunk;

/*    if ( ( chunk = stream.read() ) != null )
        do njsq._wrapper(xppq,onRead, stream, chunk);
        while ( ( chunk = stream.read() ) != null);
    else
        njsq._wrapper(xppq,onEnd, stream);
*/

     if ( ( chunk = stream.read() ) != null )
     	njsq._wrapper(xppq, onData, stream, chunk);
     else
     	njsq._wrapper(xppq, onEnd, stream);
}

var modes = {
    READABLE: 0,
    DATA_END: 1
};

function overload( mode, stream, onData, onEnd )
{
    if ( mode == modes.READABLE )
        stream.on('readable', () => onReadable(stream, onData, onEnd) );
    else if ( mode == modes.DATA_END ) {
    	stream.on('data', (chunk) => { njsq._wrapper(xppq, onData, stream, chunk); process.stdout.write(""); }); // The 'process.stdout.write("")' will do nothing, but the parser stalls when parsing the preprocessor output if missing.
    	stream.on('end', () => njsq._wrapper(xppq, onEnd, stream));
    } else
        throw "Unknown mode..."
}

class Stream extends stream.Readable {
    _read(size) {
    	njsq._wrapper(xppq, 6, this);
//        this.push(null);
    }
    constructor(stream, options) {
        super(options);
        overload( modes.DATA_END, stream, 4, 5);
        njsq._wrapper(xppq, 7, stream, this);
    }
}

// If modified, modify also 'parser.cpp'.
var tokens = {
    ERROR: 0,
    DONE: 1,
    START_TAG: 2,
    ATTRIBUTE: 3,
    VALUE: 4,
    END_TAG: 5
};

module.exports.componentInfo = () => njsq._componentInfo(xppq);
module.exports.wrapperInfo = () => njsq._wrapperInfo();
module.exports.Stream = Stream;
module.exports.parse = (stream, callback) => { overload(modes.READABLE, stream, 1, 2); njsq._wrapper(xppq, 3, stream, callback) };
module.exports.tokens = tokens;
