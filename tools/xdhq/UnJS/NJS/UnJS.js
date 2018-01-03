/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of UnJS.

	UnJS is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	UnJS is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with UnJS. If not, see <http://www.gnu.org/licenses/>.
*/

'use strict'

var xdhqId = "";
var xdhwebqId = "";

if (process.env.EPEIOS_SRC) {
    var epeiosToolsPath = "";
    if (process.platform == 'win32')
        epeiosToolsPath = "h:/hg/epeios/tools/";
    else
        epeiosToolsPath = "~/hg/epeios/tools/";

    xdhwebqId = epeiosToolsPath + "xdhwebq/NJS/XDHWebQ.js";
    xdhqId = epeiosToolsPath + "xdhq/wrappers/NJS/XDHq.js";
} else {
    xdhwebqId = "xdhwebq";
    xdhqId = "xdhqxdh";
}

const xdhwebq = require(xdhwebqId);
const xdhq = require(xdhqId);

function launch(callback, rootDir, action) {
    xdhq.launch(callback);
    require('child_process').fork(require(xdhwebqId), [rootDir, action]);
}

module.exports.register = xdhq.register;
module.exports.launch = launch;
module.exports.Tree = xdhq.Tree;
module.exports.UnJS = xdhq.XDH;
