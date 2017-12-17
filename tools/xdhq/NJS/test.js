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

class MyData extends xdhq.XDH {
	constructor() {
		super();
		this.timestamp = new Date();
	}
}

function handleTestButton(xdh, enabled) {
	if (enabled)
		xdh.setCasts("Root", "TestButtonCasting", "Plain" );
	else
		xdh.setCasts("Root", "TestButtonCasting", "Hide" );
}

function callback() {
	console.log("Connection detected !");

	return new MyData();
}

function connect( xdh, id ) {
	var tree = new xdhq.LayoutTree();

	xdh.setLayout(tree, "../XSL/Main.xsl", () => { console.log('coucou'); } );
//	handleTestButton(xdh, false);
}

function testing(xdh, id) {
	console.log("Testing : " + xdh.timestamp);
}

function hideTestButton(xdh, id) {
	handleTestButton(xdh, false);
}

function showTestButton(xdh, id) {
	handleTestButton(xdh, true);
}

function submission(xdh, id) {
	xdh.setContent( "Pattern", xdh.getContents( "Pattern" ).toUpperCase() );
}

function main()
{
	xdhq.register("Connect", connect);	// Testing.
	xdhq.register([
		["Testing", testing],
		["ShowTestButton", showTestButton],
		["HideTestButton", hideTestButton],
		["Submission", submission],
	]);

	xdhq.listen(callback, "12345");
}

main();
