/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of xppq.

	xppq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xppq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xppq. If not, see <http://www.gnu.org/licenses/>.
*/

"use strict"

var xppq = null;

if ( process.env.EPEIOS_SRC ) {
 var addonPath = null;
 if ( process.platform == 'win32' )
  addonPath = '/build/Debug/';
 else
  addonPath = '/build/Release/';
  xppq = require( __dirname + addonPath + 'xppqnjs.node');
} else {
 const binary = require( 'node-pre-gyp' );
 const path = require( 'path' );
 const xppq_path = binary.find( path.resolve( path.join( __dirname, './package.json' ) ) );
 xppq = require( xppq_path );	
}

const stream = require( 'stream' );

class Stream extends stream.Readable {
 constructor( stream, options ) {
  super( options );
  xppq._wrapper( 0, stream, this, Buffer );
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

module.exports = xppq;
module.exports.Stream = Stream;
module.exports.parse = ( stream, callback ) => xppq._wrapper( 1, stream, callback );
module.exports.tokens = tokens;
