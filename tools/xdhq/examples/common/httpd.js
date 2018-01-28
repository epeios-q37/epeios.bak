/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

	This file is part of UnJSq.

	UnJSq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	UnJSq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with UnJSq If not, see <http://www.gnu.org/licenses/>.
*/

/*
To just launch a web server to serve requests for
JAVA, PHP, CLI XDHQ frontends. Useful during development.
Can be also used also for node frontends, but this web server
is automatically launched when the corresponding frontend
is launched.
Give the as argument directory containing the file to serve
for the corresponding web application. ('node httpd.js blank',
for example).
*/

"use strict"

const path = require('path');

var xdhwebqId = "";

if (process.env.EPEIOS_SRC) {
	var epeiosToolsPath = "";
	if (process.platform == 'win32')
		epeiosToolsPath = "h:/hg/epeios/tools/";
	else
		epeiosToolsPath = "~/hg/epeios/tools/";

	xdhwebqId = epeiosToolsPath + "xdhwebq/NJS/XDHWebQ.js";
} else {
	xdhwebqId = "xdhwebq";
}

require(xdhwebqId).launch(process.argv[2], "Connect");
