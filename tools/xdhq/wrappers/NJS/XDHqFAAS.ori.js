/*
MIT License

Copyright (c) 2017 Claude SIMON (https://q37.info/s/rmnmqd49)

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

var pAddr = "atlastk.org";
var pPort = 53700;
var wAddr = "";
var wPort = "";
var instances = {};

function REPLit(url) {
	require('http').createServer(function (req, res) {
//		res.end("<html><body><iframe style=\"border-style: none; width: 100%;height: 100%\" src=\"" + url + "\"</iframe></body></html>");
		res.end("<html><body><iframe style=\"border-style: none; width: 100%;height: 100%\" src=\"https://atlastk.org/repl_it.php?url=" + url + "\"</iframe></body></html>");
	}).listen(8080);
}

function getEnv(name, value) {
	let env = process.env[name];

	if (env)
		return env.trim();
	else if (value)
		return value.trim();
	else
		return "";
}

switch (getEnv("ATK") ) {
case 'DEV':
	pAddr = "localhost";
	wPort = "8080";
    console.log("\tDEV mode !");
	break;
case 'TEST':
    console.log("\tTEST mode !");
	break;
case '':case 'REPLit':
	break;
default:
	throw "Bad 'ATK' environment variable value : should be 'DEV' or 'TEST' !";
	break;
}

pAddr = getEnv("ATK_PADDR", pAddr);
pPort = parseInt(getEnv("ATK_PPORT", pPort.toString()));
wAddr = getEnv("ATK_WADDR", wAddr);
wPort = getEnv("ATK_WPORT", wPort);

if (wAddr === "")
	wAddr = pAddr;

if (wPort !== "")
	wPort = ":" + wPort;

const shared = require('./XDHqSHRD.js');
const net = require('net');

const types = shared.types;
const open = shared.open;

const mainProtocolLabel = "8d2b7b52-6681-48d6-8974-6e0127a4ca7e";
const mainProtocolVersion = "0";

const faasProtocolLabel = "7b4b6bea-2432-4584-950b-e595c9e391e1";
const faasProtocolVersion = "0";

function byteLength(str) {
	// returns the byte length of an utf8 string
	let s = str.length;
	for (let i = str.length - 1; i >= 0; i--) {
		let code = str.charCodeAt(i);
		if (code > 0x7f && code <= 0x7ff) s++;
		else if (code > 0x7ff && code <= 0xffff) s += 2;
		if (code >= 0xDC00 && code <= 0xDFFF) i--; //trail surrogate
	}
	return s;
}

class Proxy {
	get_() {
		let buffer;
		let data = Buffer.alloc(0);
	
		while (null !== (buffer = this.socket_.read()))
			data = Buffer.concat([data, buffer]);
	
		if (data.length === 0)
			throw "Connection lost!";
	
		console.log("G: ", data.length);

		return data;
	}
	
	constructor(socket) {
		this.socket_ = socket;
		this.size_ = 0;
	}

	get(size) {
		let data = Buffer.alloc(0);

		console.log("S: ", size)

		while ( size > 0 ) {
			if ( this.size_ === 0 ) {
				this.data_ = this.get_();
				this.size_ = this.data_.length;
			}

			if ( this.size_ > size )  {
				data = Buffer.concat([data,this.data_.subarray(0, size)]);
				this.data_ = this.data_.subarray(size);
				this.size_ -= size;
				size = 0;
			} else {
				data = Buffer.concat([data,this.data_.subarray(0, this.size_)]);
				size -= this.size_;
				this.size_ = 0;
			}
		}

		return data;
	}

	write(data) {
		this.socket_.write(data);
	}

	socket() {
		return this.socket_;
	}
}

function readUInt(proxy) {
	let byte = proxy.get(1)[0];
	let value = byte & 0x7f;

	while (byte & 0x80) {
		byte = proxy.get(1)[0];

		value = (value << 7) + (byte & 0x7f);
	}

	return value;
}

function readSInt(proxy) {
	let value = readUInt(proxy);

	return value & 1 ? -( ( value >> 1 ) + 1 ) : value >> 1;
}

function getString(proxy) {
	return proxy.get(readUInt(proxy)).toString("utf-8");
}

function getStrings(proxy) {
	let amount = readUInt(proxy);
	let strings = new Array();

	while (amount--) {
		strings.push(getString(proxy));
	}

	return strings;
}

function convertUInt(value) {
	let result = Buffer.alloc(1, value & 0x7f);
	value >>= 7;

	while (value !== 0) {
		result = Buffer.concat([Buffer.alloc(1, (value & 0x7f) | 0x80), result]);
		value >>= 7;
	}

	return result;
}

function convertSInt(value) {
	return convertUInt(value < 0 ? ( ( -value - 1 ) << 1 ) | 1 : value << 1);
}

function sizeEmbeddedString(string) {
    return Buffer.concat([convertUInt(byteLength(string)), Buffer.from(string, 'utf8')]);
}

function addString(data, string) {
	return Buffer.concat([data, sizeEmbeddedString(string)]);
}

function addStrings(data, strings) {
	let i = 0;
	data = Buffer.concat([data, convertUInt(strings.length)]);

	while (i < strings.length)
		data = addString(data, strings[i++]);

	return data;
}

function handleString(string) {
	let data = new Buffer(0);

	data = addString(data, string);

	return data;
}

function getResponse(proxy, type) {
	switch (type) {
		case types.UNDEFINED:
			throw "This function should not be called with UNDEFINED type !!!";
			break;
		case types.VOID:
			throw "The VOID type should be handled upstream !!!";
			break;
		case types.STRING:
			return getString(proxy);
			break;
		case types.STRINGS:
			return getStrings(proxy);
			break;
		default:
			throw "Unknown response type !!!";
			break;
	}
}

function getToken() {
	return getEnv("ATK_TOKEN");
}

let token = getToken();

if (token !== "" )
	token = "&" + token;

function standBy(proxy, instance) {
  	proxy.write(addString(convertSInt(instance._xdh.id), "#StandBy_1"));
}

function isTokenEmpty() {
	return ( token === "" ) || ( token.charAt( 0 ) === '&' );
}

function createInstance(id, socket, createCallback) {
	let instance = createCallback();

	instance._xdh = new Object;

	instance._xdh.id = id;
	instance._xdh.socket = socket;
	instance._xdh.isFAAS = true;
	instance._xdh.type = types.UNDEFINED;
	instance._xdh.handshakeDone = false;

	return instance;
}

function instanceHandshake(instance, proxy) {
	console.log("4");

	let errorMessage = getString(proxy);
	console.log("5");

	if (errorMessage !== "")
		throw (errorMessage);

	console.log("6");

	getString(proxy);

	instance._xdh.handshakeDone = true;
}

function callCallback_(callback, instance, id, action) {
	switch (callback.length) {
		case 0:
			return callback();
		case 1:
			return callback(instance);
		case 2:
			return callback(instance, id);
		default:
			return callback(instance, id, action);
	}
}

function handleInstance(instance, callbacks, proxy) {
	let cont = true;

	if (instance._xdh.type === types.UNDEFINED) {
		let id, action;

		id = getString(proxy);
		action = getString(proxy);

		console.log(action, id);

		if ((action === "") || !("_PreProcess" in callbacks) || callCallback_(callbacks("_Preprocess", instance, id, action)))
			if (callCallback_(callbacks[action], instance, id, action) && ("_PreProcess" in callbacks))
				callCallback_(callbacks("_Postprocess", instance, id, action));

		if (instance._xdh.type === types.UNDEFINED) {
			cont = false;
			standBy(proxy, instance);
		} else
			cont = instance._xdh.type === types.VOID;
	}

	while (cont) {	// Pending callbacks are handled as long as they don't have a return value.
		if (instance._xdh.callback !== undefined) {
			let type = instance._xdh.type;
			instance._xdh.type = types.UNDEFINED;
			if (type === types.VOID)
				instance._xdh.callback();
			else
				instance._xdh.callback(getResponse(proxy, type));

			if (instance._xdh.type === types.UNDEFINED) {
				cont = false;
				standBy(proxy, instance);
			} else if (instance._xdh.type !== types.VOID)
				cont = false;
		} else {
			if (instance._xdh.type !== types.VOID)
				getResponse(proxy, instance._xdh.type);

			instance._xdh.type = types.UNDEFINED;
			cont = false;
			standBy(proxy, instance);
		}
	}
}

function serve(proxy, createCallback, callbacks) {
	let id = 0;

	console.log("Serve");
	
	id = readSInt(proxy);

	console.log("Id: ", id);

	if (id === -1)	// Should nerver happen.
		throw "Received unexpected undefined command id!";
	else if (id === -2) {  //# Value reporting a new front-end.
		id = readSInt(proxy)  // The id of the new front-end.

		if (id in instances)
			throw "Instance of id  '" + id + "' exists but should not !";

		instances[id] = createInstance(id, proxy.socket(), createCallback);
		proxy.write(addString(addString(convertSInt(id), mainProtocolLabel), mainProtocolVersion));
	} else if (id == -3) {	// Instructs to close a session.
		id = readSInt(proxy)  // The id of the session to close.

		if ( !(id in instances ) )
			throw "Instance of id '" + id + "' not available for destruction!";

		delete instances[id];
	} else if ( !(id in instances ) )
		throw "Unknown instance of id '" + id + "'!";
	else  if (!instances[id]._xdh.handshakeDone) {
		console.log("1");
		instanceHandshake(instances[id], proxy);
		console.log("2");
		proxy.write(addString(convertSInt(id), "NJS"));
		console.log("3");
	} else
		handleInstance(instances[id], callbacks, proxy);
}

function ignition(proxy, createCallback, callbacks) {
	token = getString(proxy);

	if (isTokenEmpty())
		throw getString(proxy);	// Displays error message.

	if (wPort !== ":0") {
		let completeURL = getString(proxy);

		console.log(completeURL);
		console.log(new Array(completeURL.length + 1).join('^'));
		console.log("Open above URL in a web browser. Enjoy!\n");

		if (getEnv("ATK") === "REPLit") {
			REPLit(completeURL);
//			console.log("\nIF THE PROGRAM DOES NOT WORK PROPERLY, PLEASE SEE http://q37.info/s/zbgfjtp9");
			console.log("IF THE PROGRAM DOES NOT WORK PROPERLY, YOU PROBABLY FORGOT TO FORK!");
			console.log("See http://q37.info/s/zbgfjtp9 for more details.");

		} else
			open(completeURL);
	}

	proxy.socket().on('readable', () => serve(proxy, createCallback, callbacks));
}

function faasHandshake(proxy, createCallback, callbacks, head) {
	let error = "";
	let notification = "";

	error = getString(proxy);

	if (error !== "")
		throw error;

	notification = getString(proxy);

	proxy.socket().once('readable', () => ignition(proxy, createCallback, callbacks));

	if (notification !== "")
		console.log(notification);

	proxy.write(handleString(token));

	if (head === undefined)
		head = "";

	proxy.write(handleString(head));
	proxy.write(handleString(wAddr));
}

function pseudoServer(createCallback, callbacks, head) {
	let socket = new net.Socket();

	socket.on('error', (err) => {
		throw "Unable to connect to '" + pAddr + ":" + pPort + "' !!!";
	});

	console.log("Connecting to '" + pAddr + ":" + pPort + "'…");

	socket.connect(pPort, pAddr, () => {
		console.log("Connected to '" + pAddr + ":" + pPort + "'.")
		socket.once('readable', () => faasHandshake(new Proxy(socket), createCallback, callbacks, head));
		
		socket.write(handleString(faasProtocolLabel));
		socket.write(handleString(faasProtocolVersion));

	});
}

function launch(createCallback, callbacks, head) {
	if (process.env.EPEIOS_SRC) {
		console.log("FAAS mode !");
	}

	setTimeout(() => pseudoServer(createCallback, callbacks, head), 0);
}

function addTagged(data, argument) {
	if (typeof argument === "string") {
		return addString(Buffer.concat([data,convertUInt(types.STRING)]), argument);
    } else if (typeof argument === "object") {
		return addStrings(Buffer.concat([data,convertUInt(types.STRINGS)]), argument);
    } else
		throw "Unexpected argument type: " + typeof argument;
}

function call(instance, command, type) {
	let i = 3;
	let data = convertSInt(instance._xdh.id);
	let amount = arguments.length-1;
    
    data = Buffer.concat([addString(data,command),convertUInt(type)])

//	console.log( Date.now(), " Command: ", command, instance._xdh.id);

	instance._xdh.type = type;

	while (i < amount)
		data = addTagged(data, arguments[i++]);
    
    data = Buffer.concat([data, convertUInt(types.VOID)]) // To report end of argument list.

	instance._xdh.callback = arguments[i++];

	instance._xdh.socket.write(data);
}

module.exports.launch = launch;
module.exports.call = call;
