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

//		console.log("G: ", data);

		return data;
	}
}

var stack = new Array();

var uInt = 0;
var sInt = 0;
var length = 0;
var buffer = Buffer.alloc(0);
var string = "";
var amount = 0;
var strings = "";

const d = {
	UINT: 'a',
	SINT: 'b',
	LENGTH: 'c',
	CONTENT: 'd',
	STRING: 'e',
	AMOUNT: 'f',
	SUBSTRING: 'g',
	STRINGS: 'h'
}

function push(op) {
	stack.push(op);

	switch ( op ) {
	case d.STRING:
		buffer = Buffer.alloc(0);
		push(d.LENGTH);
		break;
	case d.LENGTH:
		length = 0;
		push(d.UINT);
		break;
	case d.UINT:
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

function handleData(feeder) {
	switch( top() ) {
	case d.UINT:	// a, loop.
		if ( !handleUInt(feeder) )
			pop();
		break;
	case d.LENGTH:	// c.
		length = uInt;
		pop();
		push(d.CONTENT);
		break;
	case d.CONTENT:	// d, loop.
		if ( !handleContent(feeder) )
			pop();
		break;
	case d.STRING:	// e.
		string = buffer.toString("utf-8");
		pop();
		break;
	default:
		return false;
		break;
	}

	return true;
}

var cont = true;

const i = {
	IGNITION: '1',
	TOKEN: '2',
	ERROR: '3',
	URL: '4'
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

function ignition(feeder) {
	while ( !feeder.isEmpty() || cont ) {
		cont = false;
		switch( top() ) {
		case i.IGNITION:	// 4, out.
			return false;
			break;
		case i.TOKEN:	// '5', out.
			token = string;

			pop();

			if ( isTokenEmpty() )
				push(i.ERROR);
			else {
				push(i.URL);
				push(d.STRING);
			}
			break;
		case i.ERROR:	// 9, out.
			throw string;
			break;
		case i.URL:	// 7, out.
			pop();
			handleURL(string);
			break;
		default:
			if ( !handleData(feeder) )
				throw "Unknown ignition operation!"
			break;
		}
	}

	return true;
}

const h = {
	HANDSHAKE: '1',
	ERROR: '2',
	NOTIFICATION: '3',
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


function handshake(feeder, socket, head) {
	while ( !feeder.isEmpty() || cont ) {
		cont = false;
		switch( top() ) {
		case h.HANDSHAKE:	// 1.
			pop();
			push(i.IGNITION);
			push(i.TOKEN);
			push(d.STRING);
			return false;
			break;
		case h.ERROR:	// 2.
			if ( string.length )
				throw string;

			pop();
			push(h.NOTIFICATION);
			push(d.STRING);
			break;
		case h.NOTIFICATION:	// 3.
			if ( !handleNotification(socket, string, head) )
				pop();
			break;
		default:
			if ( !handleData(feeder) )
				throw "Unknown handshake operation!";
			break;
		}
	}

	return true;
}

const s = {
	HANDSHAKE: '1',
	IGNITION: '2',
	SERVE: '3',
}

var step = s.HANDSHAKE;

function onRead(data, socket, createCallback, callbacks, head) {
	let feeder = new Feeder(data);

	while ( !feeder.isEmpty() )
		switch ( step ) {
		case s.HANDSHAKE:
			if ( !handshake(feeder, socket, head) )
				step = s.IGNITION;
			break;
		case s.IGNITION:
			if ( !ignition(feeder) )
				step = s.SERVE;
			break;
		case s.SERVE:
			throw "SERVE";
			break;
		}
}

function launch(createCallback, callbacks, head) {
	let socket = new net.Socket();

	socket.on('error', (err) => {
		throw "Unable to connect to '" + pAddr + ":" + pPort + "' !!!";
	});

	console.log("Connecting to '" + pAddr + ":" + pPort + "'…");

	socket.connect(pPort, pAddr, () => {
		console.log("Connected to '" + pAddr + ":" + pPort + "'.")
		push(h.HANDSHAKE);
		push(h.ERROR);
		push(d.STRING);
		socket.on('data', (data) => onRead(data, socket, createCallback, callbacks, head));
		
		socket.write(handleString(faasProtocolLabel));
		socket.write(handleString(faasProtocolVersion));
	});	
}

module.exports.launch = launch;

