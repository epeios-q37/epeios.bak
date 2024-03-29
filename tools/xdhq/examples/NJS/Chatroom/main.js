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

let atlas;

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

let pseudos = [];
let messages = [];

class MyData extends DOM {
	constructor() {
		super();
		this.timestamp = new Date();
		this.pseudo = "";
		this.lastMessage = 0;
	}
}

function displayMessages(dom) {
	let xml = atlas.createXML('XDHTML');
	let i = messages.length - 1;
	let message;
	
	if (i >= dom.lastMessage) {
		xml.pushTag("Messages");
		xml.putAttribute( 'pseudo', dom.pseudo );

		while (i >= dom.lastMessage) {
			message = messages[i];
			xml.pushTag('Message');
			xml.putAttribute( 'id', i );
			xml.putAttribute( 'pseudo', message["pseudo"] );
			xml.putValue(message["content"]);
			xml.popTag();
			i--;
		}

		dom.lastMessage = messages.length;

		xml.popTag();

		dom.begin("Board", xml, "Messages.xsl");
	} else
		dom.drop()
}

function newSession() {
	return new MyData();
}

function acConnect(dom, id) {
	dom.inner("", readAsset("Main.html"),
		() => dom.focus("Pseudo",
			() => displayMessages(dom)
		)
	);
}

function handlePseudo( pseudo ) 
{
	if (pseudos.includes(pseudo))
		return false;
	else {
		pseudos.push(pseudo);
		return true;
	}
}

function acSubmitPseudo(dom, id) {
	dom.getValue("Pseudo",
		(result) => {
			result = result.trim();

			if (result.length === 0) {
				dom.alert("Cannot be empty!",
					() => dom.setValue("Pseudo", "",
						() => dom.focus( "Pseudo" )
					)
				);
			} else if (handlePseudo(result.toUpperCase())) {
				dom.pseudo = result;
				dom.addClass("PseudoButton", "hidden",
					() => dom.disableElements(["Pseudo", "PseudoButton"],
						() => dom.enableElements(["Message", "MessageButton"],
							() => dom.setValue("Pseudo", result,
								() => dom.focus("Message")
							)
						)
					)
				)
				console.log("\t>>>> New user: " + result);
			} else {
				dom.alert("Already used!",
					() => dom.setValue("Pseudo", result,
						() => dom.focus( "Pseudo" )
					)
				);
			}
		}
	);
}

function acSubmitMessage(dom, id) {
	dom.getValue("Message",
		(result) => {
			dom.setValue("Message", "",
				() => dom.focus("Message",
					() => {
						result = result.trim();
						if (result.length !== 0) {
							console.log("'" + dom.pseudo + "' : " + result);
							messages.push({
								"pseudo": dom.pseudo,
								"content": result
							});
							displayMessages(dom);
							atlas.broadcastAction("Update");
						}
					}
				)
			);
		}
	);
}

function main() {
	const CALLBACKS = {
		"": acConnect,
		"SubmitPseudo": acSubmitPseudo,
		"SubmitMessage": acSubmitMessage,
		"Update": (dom) => displayMessages(dom)
	};

	atlas.launch(newSession, CALLBACKS, readAsset( "Head.html") );
}

main();
