/*
MIT License

Copyright (c) 2018 Claude SIMON (https://q37.info/s/rmnmqd49)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

"use strict";

var atlas;

if (process.env.Q37_EPEIOS) {
	let epeiosPath = "";

	if (process.platform === 'win32')
		epeiosPath = "h:/hg/epeios/";
	else
		epeiosPath = process.env.Q37_EPEIOS;

	atlas = require(epeiosPath + "tools/xdhq/Atlas/NJS/Atlas.js");
} else {
	atlas = require('atlastk');
}

const DOM = atlas.DOM;

const readAsset = atlas.readAsset;

class Test extends DOM {
	constructor() {
		super();
		this.timestamp = new Date();
	}
}

function newSession() {
	return new Test();
}

function acConnect(dom, id) {
	dom.setLayout("", readAsset( "Main.html"),
		() => dom.lastChild("Main",
			id => dom.scrollTo(id,
				() => dom.firstChild("Main",
					id => dom.toggleClass(id,"test",
						() => dom.previousSibling("Middle",
							id => dom.toggleClass(id,"test",
								() => dom.nextSibling("Middle",
									id => dom.toggleClass(id,"test",
										() => dom.firstChild("Main",
											id => dom.scrollTo(id)
										)
									)
								)
							)
						)
					)
				)
			)
		)
	);
}

function main() {
	const callbacks = {
		"": acConnect,
	};

	atlas.launch(newSession, callbacks, readAsset( "Head.html") );
}

main();
