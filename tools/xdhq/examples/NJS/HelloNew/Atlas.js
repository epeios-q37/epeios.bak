/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

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

const path = require('path');

var xdhqId = "";
var xdhwebqId = "";
var xdhelcqPath = "";
var xdhelcqBin = "";
var electronBin = "";

function isDev() {
	if (process.env.EPEIOS_SRC)
		return true;
	else
		return false;
}

function getEpeiosPath() {
	if (isDev) {
		if (process.platform == 'win32') {
			return "h:/hg/epeios/"
		} else {
			return "~/hg/epeios/"
		}
	} else
		throw "Error !";
}

function getRealDir(dir) {
	if (isDev()) {
		let epeiosPath = getEpeiosPath();
		return path.resolve(epeiosPath, "tools/xdhq/examples/common/", path.relative(path.resolve(epeiosPath, "tools/xdhq/examples/NJS/"), path.resolve(dir)));	// No final '/'.
	} else
		return path.resolve(dir);
getsize}

if (isDev()) {
	let epeiosToolsPath = "";
	let binPath = "";
	if (process.platform == 'win32') {
		epeiosToolsPath = "h:/hg/epeios/tools/";
		binPath = "h:/bin/";
	} else {
		epeiosToolsPath = "~/hg/epeios/tools/";
		binPath = ~/bin/
	}

	xdhqId = epeiosToolsPath + "xdhq/wrappers/NJS/XDHq.js";
	xdhwebqId = epeiosToolsPath + "xdhwebq/NJS/XDHWebQ.js";
	xdhelcqPath = epeiosToolsPath + "xdhelcq/";
	xdhelcqBin = path.join(binPath, "xdhqxdh");
	electronBin = xdhelcqPath + "node_modules/electron/dist/electron";
} else {
	xdhqId = "xdhqnjs";
	xdhwebqId = "xdhwebq";
	xdhelcqPath = path.dirname(require.resolve("xdhelcq"));
	xdhelcqBin = require('xdhqxdh').fileName;
	electronBin = require("xdhelcq").electron;
}

const xdhq = require(xdhqId);

function launchWeb(dir) {
	require('child_process').fork(require(xdhwebqId).fileName, [dir]);
}

function launchDesktop(dir) {
	require('child_process').spawn(electronBin, [path.join(xdhelcqPath, "index.js"), "-m=" + xdhelcqBin, dir]).on('close', function (code) {
		process.exit(code)
	});
}

const types = {
	NONE: 0,
	DESKTOP: 1,
	WEB: 2,
	DESKTOP_AND_WEB: 3
}

module.exports.types = types;

const defaultType = types.DESKTOP;

function getSize( query, offset ) {
	var size = query[offset];

	return size;
}

function getString( query, offset ) {
	var size = getSize( query, offset++ );
	var string = "";

	while ( size-- )
		string += String.fromCodePoint( query[offset++]);

	return string;
}

function hasToLaunch( query ) {
	var i = 0;
	var request = "";

	while( query[i] != 0 )
		request += String.fromCodePoint(query[i++]);

 if ( request == "Ready_1")
	 return false;
	else if ( request == "Launch_1")
	 return true;
	else
	 throw "Unknown query: '" + request + "'";
}

function getQuery( socket, action, id ) {
	var buffer;
	var query = Buffer.alloc(0);

	while ( buffer = socket.read() )
	 query = Buffer.concat( [query, buffer] );

	return query;	
}

function getId( query ) {
	return getString( query, 9 );
}

function getAction( query ) {
	return getString( query, 9 + getSize( query, 9 ) + 1 );
}

function launch(create, callback, callbacks, type) {
	var dir = getRealDir(path.dirname(process.argv[1]));

	if (type === undefined) {
		if (process.argv.length > 2) {
			switch (process.argv[2]) {
				case "n":
				case "none":
					type = types.NONE;
					break;
				case "d":
				case "desktop":
					type = types.DESKTOP;
					break;
				case "web":
				case "w":
					type = types.WEB;
					break;
				case "dw":
				case "wd":
					type = types.DESKTOP_AND_WEB;
					break;
				default:
					throw ("Unknown type !");
					break;
			}
		} else
			type = defaultType;
	}

//	xdhq.launch(callback, action);
const server = net.createServer((c) => {
  // 'connection' listener
//		c.on('data', function(chunk) {console.log( ">" + chunk.toString() + "<" ) });
c.on('readable',() => 
	{

  if ( c._dom === undefined ) {
		 var data;

		 while ( data = c.read() )
		  console.log( ">" + data + ' ||| ' + Buffer.from(data) + "<" );

	  c._dom = create( c );
			c._dom._callbacks = callbacks;
			c._dom._socket = c;

			c.write( Buffer.from( "OK_1\x00"));
	} else {
		var query;

		query = getQuery( c );

		console.log( "Query: ", query.toString() );

		if ( hasToLaunch( query ) ) {
		 var id, action;

			id = getId( query );
			action = getAction( query );
		 console.log( "action: '" + action + "', id: '" + id + "'");

			if( action == "") {
				callback( c._dom, "" );
				c.write( Buffer.from( "OK_1\x00"));
			}
		}

			c.write( Buffer.from( "OK_1\x00"))
	}
	}
);

  console.log('client connected');
//  c.write('hello\r\n');
//  c.pipe(c);
});
server.on('error', (err) => {
  throw err;
});
server.listen(53752, () => {
  console.log('server bound');
});


	switch (type) {
		case types.NONE:
			break;
		case types.DESKTOP:
			launchDesktop(dir);
			break;
		case types.WEB:
			launchWeb(dir);
			break;
		case types.DESKTOP_AND_WEB:
			launchDesktop(dir);
			launchWeb(dir);
			break;
		default:
			throw ("Unknown type !");
			break;
	}
}

const net = require('net');


function call( socket, callback, command ) {
	// console.log( arguments.length, arguments );

	var i = 3;
	var  data = Buffer.from( command + '\x00');

	while ( i < arguments.length ) {
		console.log( arguments[i], typeof( arguments[i]));
		data = Buffer.concat( [data, Buffer.alloc(1,arguments[i].length), Buffer.from(arguments[i])])
//		data += parseInt(arguments[i].length) + arguments[i];
		i++;
	}

	console.log( data );
	socket.write( data );
}

class DOM {
	setLayout(id, tree, xslFilename, callback) {
		call( this._socket, callback, "SetLayout_1", id, tree.end(), xslFilename);
	}
}

module.exports.register = xdhq.register;
module.exports.launch = launch;
module.exports.createTree = xdhq.createTree;
module.exports.DOM = DOM;
