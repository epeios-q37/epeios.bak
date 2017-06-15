/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of esketch.

	esketch is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	esketch is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with esketch. If not, see <http://www.gnu.org/licenses/>.
*/

"use strict"

var affix = "esketch";

// Begin of generic part.
var njsq = null;
var addonPath = null;
var path = require("path");

if (process.env.EPEIOS_SRC) {
    if (process.platform == 'win32')
        addonPath = 'h:/bin/';
    else
        addonPath = '~/bin/';
    njsq = require(addonPath + 'njsq.node');
} else {
    njsq = require('njsq');
    addonPath = __dirname;
}

njsq.register(path.join(addonPath, affix + "njs" ).replace(/\\/g, "\\\\").replace(/'/g, "\\'"));
module.exports = njsq;
// End of generic part.

module.exports.returnArgument = (text) => { return njsq._wrapper(0, text) };

