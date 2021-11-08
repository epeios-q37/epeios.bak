/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

// Displays some debug log when uncommented.
// Can be activated programmatically with 'atlastk.debugLog()'.
// var debugLog = true;

var before = 0;
var cgi = "";
var session = "";
var target = "";

// By using 'geval' instead of directly 'eval',
// variable defined in the 'geval' executed script
// are of global scope, instead of local scope…
var geval = eval

const exit = {
	values : {
	NONE: 0,		// The software is currently running.
	// If below is modified, also modify the 'ErrorScript' entry in the 'xdhwebq.xcfg" file.
	TOKEN: 1,		// Token does not exists.
	BACKEND: 2,	// Backend has been shut down.
	SOCKET: 3,	// Socket has been closed (websocket timeout, for example).
	},
	messages : {
		NONE: `
		<span>Unknown exit value (<a href="http://q37.info/s/7mdgqk4z" target="_blank">exit message #0</a>)!!!</span>
		`,	// This one is ued for unknown exit value.
			TOKEN: `
		<span>No (more) application corresponding to given token (<a href="http://q37.info/s/7mdgqk4z" target="_blank">exit message #1</a>)!</span>
			`,
			BACKEND: `
		<span>Application halted! The backend has been shut down (<a href="http://q37.info/s/7mdgqk4z" target="_blank">exit message #2</a>)!</span>	
		`,
			SOCKET: `
		<span>You are disconnected from the backend (<a href="http://q37.info/s/7mdgqk4z" target="_blank">exit message #3</a>)!</span>
		<p></p>
		<button onclick="window.parent.document.location.reload()">Reload</button>
		`,
	}
};

var exitValue = exit.values.NONE;

var queryInProgress = false;
var queryQueue = [];

function log(message) {
	if ( ( typeof debugLog === 'undefined' ) || !debugLog)
		return;

	if (navigator.userAgent.search("Edge") === -1)	// MS Edge web browser does not like too much log messages...
		console.log(message);
}

function buildQuery() {
	return cgi + "?_session=" + session + "&_target=" + target + "&";
}

function t(s) {
	d = new Date();
	log(s + " : " + String(d.getTime() - before));
}

var socket;

function launchEvent(digest) {
	if (queryInProgress) {
		if (digest !== queryQueue[queryQueue.length - 1]) {
			log("Queued: ", digest);
			queryQueue.push(digest);
		}
	} else {
		log("Sent: ", digest);
		queryInProgress = true;
		socket.send(digest);
	}
}

function adjustExitIFrameHeight() {
	let iframe = window.parent.document.body.firstElementChild.firstElementChild.firstElementChild;
	iframe.height = "1";
	iframe.height = iframe.contentWindow.document.body.scrollHeight + 'px';
}

function displayExit(html) {
	let wrappedHTML = '<span style="border-radius: 25px; padding: 10px;border: 5px solid red; background-color: bisque; text-align: center; display: table; margin: auto;">' + html + "</span>";
	let src = "Exit.php?text=" + encodeURIComponent(btoa(wrappedHTML));
	document.body.firstElementChild.insertAdjacentHTML('afterbegin','<div style="width: 100%;"><iframe style="border: none;" width="100%" src="' + src + '"></iframe></div>');
	window.scroll(0,0);
}

function connect(token, id) {
	socket = new WebSocket((window.location.protocol === "http:" ? "ws" : "wss") + "://" + window.location.hostname + "/xdh/");

	socket.onopen = function (e) {
		socket.send(token);
		socket.send(id);
	}

	socket.onmessage = function (event) {
		if (event.data !== "%StandBy") {
			if (event.data === "%Quit") {	// Only used in 'FaaS' mode, when quitting a backend.
				log("Quitting !");
				exitValue = exit.values.BACKEND;
				socket.close();	// Launches 'onclose' event.
			} else {
				log("Executed:" + event.data);
				let result = geval(event.data);

				if ((typeof result !== "undefined") && (typeof result !== "object"))	// 'typeof result !== "object"' == 'result != null' !!!!
					socket.send(result);
			}
		} else if (queryQueue.length) {
			log("Unqueued:", queryQueue[0]);
			socket.send(queryQueue.shift());
		} else {
			log("Standby !");
			queryInProgress = false;
		}
	};

	socket.onclose = function (event) {
		let message = "???";

		if ( exitValue === exit.values.NONE )	// Probably a websocket timeout.
			exitValue = exit.values.SOCKET;

		switch ( exitValue ) {
		case exit.values.TOKEN:
			message = exit.messages.TOKEN;
			break;
		case exit.values.BACKEND:
			message = exit.messages.BACKEND;
			break;
		case exit.values.SOCKET:
			message = exit.messages.SOCKET
			break;
		default:
			message = exit.messages.NONE;
			break;
		}

		displayExit(message);
	}
}
