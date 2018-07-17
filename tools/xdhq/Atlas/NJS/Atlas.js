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
}

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

const modes = xdhq.modes;

function launchWeb(dir) {
	require('child_process').fork(require(xdhwebqId).fileName, [dir]);
}

function launchDesktop(dir,prod) {
	if (prod) {
		require('child_process').spawn(electronBin, [path.join(xdhelcqPath, "index.js"), "-m=" + xdhelcqBin, dir]).on('close', function (code) {
			process.exit(code)
		});
	} else {
		require('child_process').spawn(electronBin, [path.join(xdhelcqPath, "index.js"), "-s=localhost:53752", "-m=" + xdhelcqBin, dir]).on('close', function (code) {
			process.exit(code)
		});
	}

}

const guis = {
	NONE: 0,
	DESKTOP: 1,
	WEB: 2,
	DESKTOP_AND_WEB: 3
}

module.exports.guis = guis;

const defaultGUI = guis.DESKTOP;

var mode;

if (process.env.EPEIOS_SRC) {
	mode = modes.PROD;
} else {
	mode = modes.DEMO;
}

function launch(createCallback, newSessionAction, callbacks, gui) {
	var dir = getRealDir(path.dirname(process.argv[1]));
	var arg = "";
	var prod = false;

	if ( process.argv.length > 2)
		arg = process.argv[2];

	if (arg != "") {
		if (arg[0] == 'P') {
			mode = modes.PROD;
			arg = arg.substr( 1 );
		} else if (arg[0] == 'D') {
			mode = modes.DEMO;
			arg = arg.substr(1);
		}
	}

	if (gui === undefined) {
		if (arg!="") {
			switch (arg) {
				case "n":
				case "none":
					gui = guis.NONE;
					break;
				case "d":
				case "desktop":
					gui = guis.DESKTOP;
					break;
				case "web":
				case "w":
					gui = guis.WEB;
					break;
				case "dw":
				case "wd":
					gui = guis.DESKTOP_AND_WEB;
					break;
				default:
					throw ("Unknown gui !");
					break;
			}
		} else
			gui = defaultGUI;
	}

	prod = mode == modes.PROD;
	xdhq.launch(createCallback, newSessionAction, callbacks, mode);

	switch (gui) {
		case guis.NONE:
			break;
		case guis.DESKTOP:
			launchDesktop(dir,prod);
			break;
		case guis.WEB:
			launchWeb(dir);
			break;
		case guis.DESKTOP_AND_WEB:
			launchDesktop(dir,prod);
			launchWeb(dir);
			break;
		default:
			throw ("Unknown gui !");
			break;
	}
}

module.exports.launch = launch;
module.exports.createTree = () => require('xmlbuilder').create('XDHTML');
module.exports.DOM = xdhq.XDH;
