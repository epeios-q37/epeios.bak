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
	along with XDHq If not, see <http://www.gnu.org/licenses/>.
*/

var atlasId = "";

if (process.env.EPEIOS_SRC) {
	if (process.platform == 'win32')
		epeiosPath = "h:/hg/epeios/"
	else
		epeiosPath = "~/hg/epeios/"

	atlasId = epeiosPath + "tools/xdhq/Atlas/NJS/Atlas.js";
} else {
	atlasId = 'atlas';
}

const atlas = require(atlasId);

atlas.register([
    ["Connect",
        (dom, id) => dom.setLayout("", new atlas.Tree(), "Main.xsl")],
    ["Reverse",
        (dom, id) => dom.getContent("Message",
            (result) => dom.setContent("Message", result.split('').reverse().join('')))
    ]]);

atlas.launch(() => { return new atlas.DOM() }, "reverse", "Connect");
