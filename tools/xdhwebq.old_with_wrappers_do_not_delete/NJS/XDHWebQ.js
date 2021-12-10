/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

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

// NOT TO CONFUSE WITH '../js/xdhwebq.js', which is red by the web browser.

'use strict'

var rootDir = "";

const http = require('http');
const url = require('url');
const stream = require('stream');
const fs = require('fs');
const path = require('path');

var njsq = null;
var componentFilename = "";
var xdhqxdhId = "";
var xdhtmlJSPath = "";
var xdhwebqJSPath = "";
var cdnPath = "";
var selfPath = "";

function isDev() {
	if (process.env.EPEIOS_SRC)
		return true;
	else
		return false;
}

function getEpeiosPath() {
	if (isDev()) {
		if (process.platform == 'win32') {
			return "h:/hg/epeios/"
		} else {
			return "~/hg/epeios/"
		}
	} else
		throw "Error !";
}

function getComponentPath() {
	if (isDev()) {
		if (process.platform == 'win32') {
			return 'h:/bin/';
		} else {
			return '~/bin/';
		}
	} else
		return __dirname;
}

function getSelfPath() {
	if (isDev())
		return path.join(getEpeiosPath() + "tools/xdhwebq/NJS");
	else
		return __dirname;
}

if (isDev()) {
	let epeiosPath = getEpeiosPath();
	njsq = require(getComponentPath() + 'njsq.node');
	xdhtmlJSPath = epeiosPath + "corpus/js/";
	xdhwebqJSPath = epeiosPath + "tools/xdhwebq/js/";
	xdhqxdhId = epeiosPath + "tools/xdhq/proxy/LIB/XDHq.js";
} else {
	njsq = require('njsq');
	let jsPath = path.join(__dirname, "js");
	xdhtmlJSPath = jsPath;
	xdhwebqJSPath = jsPath;
	xdhqxdhId = 'xdhqxdh';
}

const xdhtmlJSFilename = path.join(xdhtmlJSPath, "xdhtml.js");
const xdhwebqJSFilename = path.join(xdhwebqJSPath, "xdhwebq.js");

componentFilename = path.join(getComponentPath(), "xdhwebqnjs").replace(/\\/g, "\\\\").replace(/'/g, "\\'").replace(/ /g, "\\ ");
const xdhwebq = njsq._register(componentFilename);
module.exports.componentInfo = () => njsq._componentInfo(xdhwebq);
module.exports.wrapperInfo = () => njsq._wrapperInfo();

function put(keysAndValues, keys, values) {
	for (var prop in keysAndValues) {
		keys.push(prop);
		values.push(keysAndValues[prop]);
	}
}

function userHead() {
	try {
		return fs.readFileSync(path.join(cdnPath, "head.html"));
	} catch (err) {
		return "";
	}
}

function prolog(token) {
	if (token == undefined)
		token = "";
	return [
		'<!DOCTYPE html>',
		'<html>',
		'	<head id="_xdh_head">',
		'		<meta name="generator" content="XDHWebQNJS"/>',
		'		<meta charset="UTF-8" />',
		'		<meta http-equiv="X-UA-Compatible" content="IE=edge" />',
		'		<script src="xdh/xdhtml.js"></script>',
		'		<script src="xdh/xdhwebq.js"></script>',
		'		<script>handleQuery("?_token=' + token + '&_action=")</script>',
		'	</head>',
		'	<body id="XDHRoot">',
		'	</body>'
	].join("\n");
}

function serveQuery(query, res) {
	var response = "";
	if ('_action' in query) {
		var keys = new Array();
		var values = new Array();

		put(query, keys, values);

		// Synchrone handling.
		new StringStream(njsq._call(xdhwebq, 2, keys, values)).pipe(res);

		// Asynchrone handling. Works, but very slow !?!
		// njsq._call(xdhwebq, 3, keys, values, (result) => new StringStream(result ).pipe(res));
	} else {
		new StringStream(prolog(query['_token'])).pipe(res);
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
	'.eot': 'application/vnd.ms-fontobject',
	'.ttf': 'application/font-sfnt'
};

function serveFile(pathname, res) {
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
				// based on the URL path, extract the file extension. e.g. .js, .doc, ...
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

	if (pathname == '/')
		serveQuery(parsedUrl.query, res);
	else if (pathname == '/xdh/xdhwebq.js')
		serveFile(xdhwebqJSFilename, res);
	else if (pathname == '/xdh/xdhtml.js')
		serveFile(xdhtmlJSFilename, res);
	else
		serveFile(`${cdnPath}${pathname}`, res);

}

function launch(dir, service) {
	cdnPath = path.resolve(dir);

	if (service === undefined)
		service = 8080;
	else if (!Number.isInteger(service))
		throw "Argument, if provided, is the port to listen to (8080 by default ) !";

	njsq._call(xdhwebq, 1, require(xdhqxdhId).fileName);

	http.createServer(
		function (req, res) {
			serve(req, res);
		}
	).listen(service).on('error', (error) => console.log("http requests served by other program"));
}

if (require.main === module) {
	// Called directly (through a fork or from CLI).
	if (process.argv.length >= 3) {
		//		console.log( process.argv);
		launch(process.argv[2], process.argv[3]);
	} else
		throw "Not enough arguments !";
} else {
	// Required as a module
}

module.exports.returnArgument = (text) => njsq._call(xdhwebq, 0, text);
module.exports.serve = serve;
module.exports.launch = launch;
module.exports.fileName = path.normalize(path.join(getSelfPath(), "XDHWebQ.js"));

