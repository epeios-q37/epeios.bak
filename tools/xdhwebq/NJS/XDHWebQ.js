/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/

"use strict"

const http = require('http');
const url = require('url');
const stream = require('stream');
const fs = require('fs');
const path = require('path');

var firstAction = "";

// Begin of generic part.
var njsq = null;
var componentPath = null;
var componentFilename = null;

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

componentFilename = path.join(componentPath, "xdhwebqnjs").replace(/\\/g, "\\\\").replace(/'/g, "\\'").replace(/ /g, "\\ ");
const xdhwebq = njsq._register(componentFilename);
module.exports.componentInfo = () => njsq._componentInfo(xdhwebq);
module.exports.wrapperInfo = () => njsq._wrapperInfo();
// End of generic part.

function put(keysAndValues, keys, values) {
	for (var prop in keysAndValues) {
		keys.push(prop);
		values.push(keysAndValues[prop]);
	}
}

function serveQuery(query)
{
	if ( ('_action' in query) && ( query['_action'] != '' ) ) {
		var keys = new Array();
		var values = new Array();

		put(query, keys, values);
		return njsq._wrapper(xdhwebq, 2, keys, values);
	} else {
		return '\
<!DOCTYPE html>\
<html>\
	<head>\
		<meta charset="UTF-8" />\
		<meta http-equiv="X-UA-Compatible" content="IE=edge">\
		<title>MegaRentals</title>\
		<meta name="description" content=""/>\
		<meta name="viewport" content="width=device-width, initial-scale=1"/>\
		<link integrity="" rel="stylesheet" href="ember/assets/vendor.css"/>\
		<style>.tomster {\
			background:url(ember/assets/q37.png) no-repeat;\
			background-size:contain;\
			height:150px;\
			width:150px;\
			top:-50px\
		}\
		</style>\
		<script src="js/xdhtml.js"></script>\
		<script src="js/xdhwebq.js"></script>\
		<script>handleQuery("?_action=' + firstAction + '")</script>\
	</head>\
	<body id="Root">\
	</body>\
</html>\
		';
	}
}

class StringStream extends stream.Readable {
	constructor(text, options) {
		super(options);
		this.text = text;
	}
	_read() {
		if (!this.eos) {
			this.push(this.text);
			this.eos = true
		} else {
			this.push(null);
		}
	}
}

function serveFile(pathname, res) {
	const mimeType = {
		'.ico': 'image/x-icon',
		'.html': 'text/html',
		'.js': 'text/javascript',
		'.json': 'application/json',
		'.css': 'text/css',
		'.png': 'image/png',
		'.jpg': 'image/jpeg',
		'.wav': 'audio/wav',
		'.mp3': 'audio/mpeg',
		'.svg': 'image/svg+xml',
		'.pdf': 'application/pdf',
		'.doc': 'application/msword',
		'.eot': 'appliaction/vnd.ms-fontobject',
		'.ttf': 'aplication/font-sfnt'
	};
	fs.exists(pathname, function (exist) {
		if (!exist) {
			// if the file is not found, return 404
			res.statusCode = 404;
			res.end(`File ${pathname} not found!`);
			return;
		}
		// if is a directory, then look for index.html
		if (fs.statSync(pathname).isDirectory()) {
			pathname += '/index.html';
		}
		// read file from file system
		fs.readFile(pathname, function (err, data) {
			if (err) {
				res.statusCode = 500;
				res.end(`Error getting the file: ${err}.`);
			} else {
				// based on the URL path, extract the file extention. e.g. .js, .doc, ...
				const ext = path.parse(pathname).ext;
				// if the file is found, set Content-type and send data
				res.setHeader('Content-type', mimeType[ext] || 'text/plain');
				res.end(data);
			}
		});
	});
}

function serve(req, res) {
	const parsedUrl = url.parse(req.url, true);
	const pathname = parsedUrl.pathname;

	if (pathname == '/') {
		new StringStream(serveQuery(parsedUrl.query)).pipe(res);
//		serveQuery(parsedUrl.query, (response) => new StringStream(response).pipe(res));
	} else
		serveFile(`.${pathname}`, res);
}

function launch( action, service) {
	if ( typeof action === "string" )
		firstAction = action;
	else
		throw ("First argument must be the name of the action to launch at connection !");

	if (service === undefined)
		service = 8080;
	else if ( !Number.isInteger( service ) )
		throw "Second argument, if provided, is the port to listen to (8080 by default ) !";

	http.createServer(function (req, res) {
		serve(req, res);
	}).listen(service);

}


njsq._wrapper(xdhwebq, 1, "h:/bin/xdhqxdh");

module.exports.returnArgument = (text) => njsq._wrapper(xdhwebq, 0, text);
module.exports.serve = serve;
module.exports.launch = launch;

