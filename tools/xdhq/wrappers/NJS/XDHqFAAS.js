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

var token = getEnv("ATK_TOKEN");

if (token !== "" )
	token = "&" + token;


function isTokenEmpty() {
	return ( token === "" ) || ( token.charAt( 0 ) === '&' );
}	

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

class Feeder {
	constructor(data) {
		this.data_ = data;
	}
	isEmpty() {
		return this.data_.length === 0;
	}
	get(size) {
		if ( this.data_.length === 0 )
			throw "No data available!";

		if ( size > this.data_.length)
			size = this.data_.length;

		if ( size === 0 )
			throw "'size' can not be 0!"

		let data = this.data_.subarray(0,size);

		this.data_ = this.data_.subarray(size);

		console.log("G: ", data);

		return data;
	}
}

const op = {
	// Action.
	HANDSHAKE: '1',
	HANDSHAKE_ERROR: '2',
	NOTIFICATION: '3',
	IGNITION: '4',
	TOKEN: '5',
	IGNITION_ERROR: '6',
	URL: '7',
	// Data.
	UINT: 'a',
	SINT: 'b',
	LENGTH: 'c',
	CONTENT: 'd',
	STRING: 'e',
	AMOUNT: 'f',
	SUBSTRING: 'g',
	STRINGS: 'h'
}

var stack = new Array();
var uInt = 0;
var sInt = 0;
var length = 0;
var buffer = Buffer.alloc(0);
var string = "";
var amount = 0;
var strings = "";
var cont = true;

function push(operation) {
	stack.push(operation);

	switch ( operation ) {
	case op.STRING:
		buffer = Buffer.alloc(0);
		push(op.LENGTH);
		break;
	case op.LENGTH:
		length = 0;
		push(op.UINT);
		break;
	case op.UINT:
		uInt = 0;
		break;
	}
}

function pop() {
	cont = true;
	return stack.pop();
}

function top() {
	return stack[stack.length - 1];
}

function handleUInt(feeder) {
	if ( feeder.isEmpty() )
		return true;

	let byte = feeder.get(1)[0];
	uInt = (uInt << 7 ) + (byte & 0x7f);

	return byte & 0x80;
}

function handleContent(feeder) {
	if ( feeder.isEmpty() )
		return true;

	if ( length !== 0 )
		buffer = Buffer.concat([buffer,feeder.get(length-buffer.length)]);

	return length !== buffer.length;
}

function handleNotification(socket, notification, head) {
	if ( notification !== "" )
		console.log(notification);

	socket.write(handleString(token));

	if (head === undefined)
		head = "";

	socket.write(handleString(head));
	socket.write(handleString(wAddr));
}

function handleURL(url) {
	console.log(url);
	console.log(new Array(url.length + 1).join('^'));
	console.log("Open above URL in a web browser. Enjoy!\n");

	if (getEnv("ATK") === "REPLit") {
		REPLit(url);
//			console.log("\nIF THE PROGRAM DOES NOT WORK PROPERLY, PLEASE SEE http://q37.info/s/zbgfjtp9");
		console.log("IF THE PROGRAM DOES NOT WORK PROPERLY, YOU PROBABLY FORGOT TO FORK!");
		console.log("See http://q37.info/s/zbgfjtp9 for more details.");

	} else
		open(url);	
}


function handle(feeder, socket, createCallbacks, callbacks, head) {
	while ( !feeder.isEmpty() || cont ) {
		cont = false;
		switch( top() ) {
		case op.HANDSHAKE:	// 1, out.
			pop();
			push(op.IGNITION);
			push(op.TOKEN);
			push(op.STRING);
			break;
		case op.IGNITION:	// 4, out.
			throw "Ignition ended";
			break;
		case op.HANDSHAKE_ERROR:	// 2, out.
			if ( string.length )
				throw string;

			pop();
			push(op.NOTIFICATION);
			push(op.STRING);
			break;
		case op.NOTIFICATION:	// 3, out.
			if ( !handleNotification(socket, string, head) )
				pop();
			break;
		case op.TOKEN:	// '5', out.
			token = string;

			pop();

			if ( isTokenEmpty() )
				push(op.IGNITION_ERROR);
			else {
				push(op.URL);
				push(op.STRING);
			}
			break;
		case op.IGNITION_ERROR:	// 9, out.
			throw string;
			break;
		case op.URL:	// 7, out.
			pop();
			handleURL(string);
			break;
		// Data handling.
		case op.STRING:	// e, out.
			string = buffer.toString("utf-8");
			pop();
			break;
		case op.LENGTH:	// c, out.
			length = uInt;
			pop();
			push(op.CONTENT);
			break;
		case op.UINT:	// a, Loop.
			if ( !handleUInt(feeder) )
				pop();
			break;
		case op.CONTENT:	// d, loop.
			if ( !handleContent(feeder) )
				pop();
			break;
		}
	}
}

function onRead(data, socket, createCallback, callbacks, head) {
	let feeder = new Feeder(data);

	handle(feeder, socket, createCallback, callbacks, head);
}

function launch(createCallback, callbacks, head) {
	let socket = new net.Socket();

	socket.on('error', (err) => {
		throw "Unable to connect to '" + pAddr + ":" + pPort + "' !!!";
	});

	console.log("Connecting to '" + pAddr + ":" + pPort + "'…");

	socket.connect(pPort, pAddr, () => {
		console.log("Connected to '" + pAddr + ":" + pPort + "'.")
		push(op.HANDSHAKE);
		push(op.HANDSHAKE_ERROR);
		push(op.STRING);
		socket.on('data', (data) => onRead(data, socket, createCallback, callbacks, head));
		
		socket.write(handleString(faasProtocolLabel));
		socket.write(handleString(faasProtocolVersion));
	});	
}

module.exports.launch = launch;

