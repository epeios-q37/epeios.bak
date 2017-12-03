/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

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

// Once installed ('npm install xdhq'), launch 'npm explore xdhq -- node test.js'.

const xdhq = require('./XDHq.js');

console.log( xdhq.componentInfo() ) ;
console.log( xdhq.wrapperInfo() );

process.stdout.write(xdhq.returnArgument("Text from JS file") + '\n');

function callback() {
	console.log("Connection detected !");

	return new xdhq.XDH();
}

function connect( xdh, id ) {
	var xml = new xdhq.Tree( "Layout" );

	xdh.setLayout("Root", xml, "../XSL/MainLayout.xsl");

	console.log("Connect : " + id );
}

function testing(xdh, id) {
	console.log("Testing : " + id);
}


function main()
{
	xdhq.register("Connect", connect);
//	xdhq.register("Testing", testing);

	xdhq.listen(callback, "12345");
}

main();
