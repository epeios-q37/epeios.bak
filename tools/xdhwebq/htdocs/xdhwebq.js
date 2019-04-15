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
		
function handleQuery( query ) {

//	log( "Q : " + query );
    var xmlhttp;

	if (window.XMLHttpRequest) {	// code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp = new XMLHttpRequest();
	} else {	// code for IE6, IE5
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
	
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState === 4 && xmlhttp.status === 200) {
            let query = "";

//          log("R : " + xmlhttp.responseText);
            let xdh_result = eval(xmlhttp.responseText);

            if (query) {
                if (typeof xdh_result !== "undefined" && typeof xdh_result !== "object")	// 'typeof xdh_result !== "object"' == 'xdh_result != null' !!!!
                    query += "&_result=" + encodeURIComponent(xdh_result);
                handleQuery(buildQuery() + query);
            } else if (queryQueue.length)
                handleQuery(queryQueue.shift());
            else
                queryInProgress = false;

        }
    };

	if ( before === 0 ) {
		d = new Date();
		before = d.getTime();
	}

	xmlhttp.open("GET", query);

	xmlhttp.send();
}

function launchEvent( digest )
{
	query = buildQuery() + "_action=_HandleEvent&digest=" + encodeURIComponent( digest );

	if (queryInProgress) {
		if (query !== queryQueue[queryQueue.length - 1])
			queryQueue.push(query);
	}  else {
		queryInProgress = true;
		handleQuery(query);
	}

	return true;
}
