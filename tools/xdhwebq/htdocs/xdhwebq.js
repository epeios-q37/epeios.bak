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

var before = 0;
var cgi = "";
var session = "";
var target = "";

var queryInProgress = false;
var queryQueue = [];
var backendLost = false;
var reportClosing = true;	// Set to 'false' int the scrip' defined in the
							// 'ErrorScript' definition entry in the 'xdhwebq.xcfg'  file. 

function log( message )
{
	if ( navigator.userAgent.search( "Edge" ) === -1 )	// MS Edge web browser does not like too much log messages...
		console.log( message );
}

function buildQuery()
{
	return cgi + "?_session=" + session + "&_target=" + target + "&";
}

function t( s )
{
	d = new Date();
	log( s + " : " + String( d.getTime() - before ) );
}
		
var socket;

function launchEvent( digest )
{
	if ( queryInProgress ) {
		if (digest !== queryQueue[queryQueue.length - 1]) {
			console.log("Queued: ", digest);
			queryQueue.push(digest);
		}
	} else {
		console.log("Sent: ", digest);
		queryInProgress = true;
		socket.send(digest);
	}
}


function connect(token,id) {
	socket = new WebSocket((window.location.protocol === "http:" ? "ws" : "wss" ) + "://" + window.location.hostname + "/xdh/");

    socket.onopen = function(e) {
		socket.send(token);
		socket.send(id);
	}

    socket.onmessage = function(event) {
		if ( event.data !== "%StandBy" ) {
			if ( event.data === "%Quit" ) {	// Only used in 'FaaS' mode, when quitting a backend.
				log("Quitting !");
				backendLost = true;
				socket.close();	// Launches 'onclose' event.
			} else {
				log("Executed:", event.data);
				let result = eval(event.data);
	//			console.log(event.data);
				
				if ( ( typeof result !== "undefined" ) && ( typeof result !== "object" ) )	// 'typeof result !== "object"' == 'result != null' !!!!
					socket.send(result);
			}
		} else if (queryQueue.length) {
			console.log("Unqueued:", queryQueue[0]);
			socket.send(queryQueue.shift());
		} else {
			console.log("Standby !");
			queryInProgress = false;
		}
	};
	
    socket.onclose = function(event) {
		if ( reportClosing )
			if ( backendLost )
				alert("Connection to backend lost!");
			else if (confirm("Disconnected!\nPress OK to reload the application."))
				location.reload();
    }	
}
