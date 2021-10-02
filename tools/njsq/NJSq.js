/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

	This file is part of NJSq.

	NJSq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	NJSq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with NJSq. If not, see <http://www.gnu.org/licenses/>.
*/

"use strict"

var njsq = null;

if ( process.env.Q37_EPEIOS ) {
 var addonPath = null;
 if ( process.platform == 'win32' )
  addonPath = 'h:/bin/';
 else
  addonPath = '/home/csimon/bin/';
 njsq = require( addonPath + 'njsq.node');
} else {
 const binary = require( 'node-pre-gyp' );
 const path = require( 'path' );
 const njsq_path = binary.find( path.resolve( path.join( __dirname, './package.json' ) ) );
 njsq = require( njsq_path );	
}

/*
Do not modify line below !!!
During development, we access directly 'njsq.node', but, in production,
we access to it through this file, so it must expose exactly the same functions
as 'njsq.node'.
*/
module.exports = njsq;