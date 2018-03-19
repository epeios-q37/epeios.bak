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
	xdhelcqBin = path.join(xdhelcqPath, "xdhqxdh");
	electronBin = require("electron");
}

const xdhwebq = require(xdhwebqId);
const xdhq = require(xdhqId);

function launchWeb(dir) {
	require('child_process').fork(require(xdhwebqId).fileName, [dir]);
}

function launchDesktop(dir) {
	require('child_process').spawn(electronBin, [xdhelcqPath, "-m=" + xdhelcqBin, dir]).on('close', function (code) {
		process.exit(code)
	});
}

const types = {
	DESKTOP: 1,
	WEB: 2,
	DESKTOP_AND_WEB: 3
}

module.exports.types = types;

const defaultType = types.DESKTOP;

function launch(callback, action, type) {
	var dir = getRealDir(path.dirname(process.argv[1]));

	if (type === undefined) {
		if (process.argv.length > 2) {
			switch (process.argv[2]) {
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

	xdhq.launch(callback, action);

	switch (type) {
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

module.exports.register = xdhq.register;
module.exports.launch = launch;
module.exports.Tree = xdhq.Tree;
module.exports.DOM = xdhq.XDH;
