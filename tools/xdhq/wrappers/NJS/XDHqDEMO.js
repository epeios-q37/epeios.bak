/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

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

"use strict"

// Types of the response.
const shared = require('./XDHqSHRD.js');

const types = shared.types;
const open = shared.open;
const service = 53800;
var host = "atlastk.org";

if (process.env.EPEIOS_SRC)
	host = "localhost";

function byteLength(str) {
	// returns the byte length of an utf8 string
	var s = str.length;
	for (var i = str.length - 1; i >= 0; i--) {
		var code = str.charCodeAt(i);
		if (code > 0x7f && code <= 0x7ff) s++;
		else if (code > 0x7ff && code <= 0xffff) s += 2;
		if (code >= 0xDC00 && code <= 0xDFFF) i--; //trail surrogate
	}
	return s;
}

function getSize(query, offset) {
	var byte = query[offset++];
	var size = byte & 0x7f;

	while (byte & 0x80) {
		byte = query[offset++];

		size = (size << 7) + (byte & 0x7f);
	}

	return [size, offset];
}

function getString(query, offset) {
	var size = 0;
	[size, offset] = getSize(query, offset);

	var string = "";

	while (size--)
		string += String.fromCodePoint(query[offset++]);

	return [string, offset];
}

function getStrings(query, offset) {
	var size = 0;
	var strings = new Array();
	var string = "";

	[size, offset] = getSize(query, offset);

	while (size--) {
		[string, offset] = getString(query, offset);
		strings.push(string);
	}

	return [strings, offset];
}

function convertSize(size) {
	var result = Buffer.alloc(1, size & 0x7f);
	size >>= 7;

	while (size != 0) {
		result = Buffer.concat([Buffer.alloc(1, (size & 0x7f) | 0x80), result]);
		size >>= 7;
	}

	return result;
}

function addString(data, string) {
	return Buffer.concat([data, convertSize(byteLength(string)), Buffer.from(string, 'utf8')]);
}

function addStrings(data, strings) {
	var i = 0;
	data = Buffer.concat([data, convertSize(strings.length)]);

	while (i < strings.length)
		data = addString(data, strings[i++]);

	return data;
}

function hasToLaunch(query) {
	var i = 0;
	var request = "";

	while (query[i] != 0)
		request += String.fromCodePoint(query[i++]);

	if (request == "Ready_1")
		return false;
	else if (request == "Launch_1")
		return true;
	else
		throw "Unknown query: '" + request + "'";
}

function getQuery(socket) {
	var buffer;
	var query = Buffer.alloc(0);

	while (buffer = socket.read())
		query = Buffer.concat([query, buffer]);

	return query;
}

function getId(query) {
	return getString(query, 9)[0];
}

function getAction(query) {
	return getString(query, 9 + getSize(query, 9)[0] + 1)[0];
}

function getResponse(query, type) {
	switch (type) {
		case types.UNDEFINED:
			throw "This function should not be called with UNDEFINED type !!!";
			break;
		case types.VOID:
			throw "The VOID type should be handled upstream !!!";
			break;
		case types.STRING:
			return getString(query, 8)[0];
			break;
		case types.STRINGS:
			return getStrings(query, 8)[0];
			break;
		default:
			throw "Unknown response type !!!";
			break;
	}
}

var token = "";
var url = "";

function pseudoServer(createCallback, newSessionAction, callbacks) {
	var client = new net.Socket();

	client.connect(service, host, () => {
		var data = new Buffer(0);
		var relaunch = true;

		data = addString(data, token);

		client.write(data);

		client.on('readable', () => {
			if (client._xdhDOM === undefined) {
				var query = getQuery(client);

				token = getString(query, 0)[0];

				if (token == "")
					throw "Bad connection information !!!";

				client._xdhDOM = createCallback(client);
				client._xdhDOM._xdhSocket = client;
				client._xdhDOM._xdhIsDEMO = true;

				client.write(Buffer.from("StandBy_1\x00"));

				if (url != "") {
					let completeURL = url + "?_token=" + token;

					if (open(completeURL))
						console.log("Open " + completeURL + " in a web browser, if not already done. Enjoy!");
					else
						console.log("Open " + completeURL + " in a web browser. Enjoy!");

					url = "";
				}

			} else if (relaunch) {
				pseudoServer(createCallback, newSessionAction, callbacks);

				while (data = client.read());

				relaunch = false;
			} else {

				var query;

				query = getQuery(client);

				if (hasToLaunch(query)) {
					var id, action;

					id = getId(query);
					action = getAction(query);

					if (action == "") {
						callbacks[newSessionAction](client._xdhDOM, "");
						//            client.write(Buffer.from("StandBy_1\x00"));
					} else {
						callbacks[action](client._xdhDOM, id);
					}
				} else {
					if (client._xdhDOM._xdhType === types.VOID) {
						if (client._xdhDOM._xdhCallback != undefined) {
							client._xdhDOM._xdhType = types.UNDEFINED;
							client._xdhDOM._xdhCallback();
							if (client._xdhDOM._xdhType === types.UNDEFINED)
								client.write(Buffer.from("StandBy_1\x00"));
						} else
							client.write(Buffer.from("StandBy_1\x00"));
					} else if (client._xdhDOM._xdhCallback != undefined) {
						var type = client._xdhDOM._xdhType;
						client._xdhDOM._xdhType = types.UNDEFINED;
						client._xdhDOM._xdhCallback(getResponse(query, type));
						if (client._xdhDOM._xdhType === types.UNDEFINED)
							client.write(Buffer.from("StandBy_1\x00"));
					} else {
						getResponse(query, client._xdhDOM._xdhType);
						client.write(Buffer.from("StandBy_1\x00"));
					}
				}
			}
		});
	});
	client.on('error', (err) => {
		throw err;
	});
}

function launch(createCallback, newSessionAction, callbacks, webURL) {
	if (process.env.EPEIOS_SRC) {
		console.log("DEMO mode !");
	}

	url = webURL;

	setTimeout(() => pseudoServer(createCallback, newSessionAction, callbacks), 2000);

}

const net = require('net');

function add(data, argument) {
	if (typeof (argument) === "string")
		return addString(data, argument);
	else if (typeof (argument) === "object")
		return addStrings(data, argument);
	else
		throw "Unexpected argument type: " + typeof (argument);
}

function call(dom, command, type) {
	var i = 3;
	var data = Buffer.from(command + '\x00');
	var amount = arguments[i++];

	dom._xdhType = type;

	while (amount--)
		data = add(data, arguments[i++]);

	amount = arguments[i++];

	while (amount--)
		data = add(data, arguments[i++]);

	dom._xdhCallback = arguments[i++];

	dom._xdhSocket.write(data);
}

module.exports.launch = launch;
module.exports.call = call;
