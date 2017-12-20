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
		this.showButton = false;
	}
}

function handleTestButton(xdh) {
	console.log("button handling");

	if (xdh.showButton )
		xdh.setCasts("Root", "TestButtonCasting", "Plain", () => { console.log("hi!");console.log('plain'); });
	else
		xdh.setCasts("Root", "TestButtonCasting", "Hide", () => { console.log('hide'); });

	console.log("Hi !");
}

function callback() {
	console.log("Connection detected !");

	return new MyData();
}

function connect( xdh, id ) {
	var tree = new xdhq.LayoutTree();

//	console.log( ":::: " + tree._q37TreeRack);

	xdh.setLayout("Root", tree, "../XSL/Main.xsl",
		() => handleTestButton( xdh ) );
}

function testing(xdh, id) {
	console.log("Testing : " + xdh.timestamp);
}

function hideTestButton(xdh, id) {
	xdh.showButton = false;
	handleTestButton(xdh);
}

function showTestButton(xdh, id) {
	xdh.showButton = true;
	handleTestButton(xdh);
}

function submission(xdh, id) {
	xdh.getContent( "Pattern",
					 (result) => {
					 	console.log("Result: " + result);
					 	xdh.setContents("Pattern", result.toUpperCase(),
										() => {
											xdh.getContent(("Pattern"), (result) => console.log(result));
											console.log("Submission");
										});
					 } );
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
