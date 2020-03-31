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
		if (digest !== queryQueue[queryQueue.length - 1])
			queryQueue.push(digest);
	} else {
		queryInProgress = true;
		socket.send(digest);
	}
}


function connect(token) {
	socket = new WebSocket((window.location.protocol === "http:" ? "ws" : "wss" ) + "://" + window.location.hostname + "/xdh/");

    socket.onopen = function(e) {
		socket.send(token)
	}

    socket.onmessage = function(event) {
		if ( event.data !== "StandBy" ) {
			let result = eval(event.data);
//			console.log(event.data);
			
			if ( ( typeof result !== "undefined" ) && ( typeof result !== "object" ) )	// 'typeof xdh_result !== "object"' == 'xdh_result != null' !!!!
				socket.send(result);
		} else if (queryQueue.length) {
			socket.send(queryQueue.shift());
		} else
			queryInProgress = false;
	};
	
    socket.onclose = function(event) {
        if (confirm("Disconnected!\nPress OK to reload the application."))
            location.reload(true);
    }	
}