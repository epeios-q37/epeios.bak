/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

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

var unjsqId = "";

if (process.env.EPEIOS_SRC) {
	if (process.platform == 'win32')
		epeiosPath = "h:/hg/epeios/"
	else
		epeiosPath = "~/hg/epeios/"

	unjsqId = epeiosPath + "tools/xdhq/UnJSq/NJS/UnJSq.js";
} else {
	unjsqId = 'unjsq';
}

const unjsq = require(unjsqId);

unjsq.register([
    ["Connect",
        (dom, id) => dom.setLayout("", new unjsq.Tree(), "Main.xsl")],
    ["Reverse",
        (dom, id) => dom.getContent("Message",
            (result) => dom.setContent("Message", result.split('').reverse().join('')))
    ]]);

unjsq.launch(() => { return new unjsq.DOM() }, "reverse", "Connect");
