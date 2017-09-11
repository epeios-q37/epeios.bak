/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
*/

// Once installed, launch 'npm explore xppq -- node test.js'.
// You can submit an additional parameter of value from '0' to '4' as id of the test to launch.

"use strict"

const fs = require('fs');
const stream = require('stream');
const xppq = require('./XPPq.js');
var indentLevel = 0;
const xml = '\
<?xml version="1.0" encoding="UTF-8"?>\n\
<SomeTag xmlns:xpp="http://q37.info/ns/xpp/" AnAttribute="SomeAttributeValue">\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue in a string</SomeOtherTag>\n\
 <xpp:define name="SomeMacro">\n\
  <xpp:bloc>Some macro content !</xpp:bloc>\n\
 </xpp:define>\n\
 <YetAnotherTag YetAnotherAttribute="YetAnotherAttributeValue">\n\
  <xpp:expand select="SomeMacro"/>\n\
 </YetAnotherTag>\n\
</SomeTag>\
';

//const xml = "<R/>";
var out = "";

class StringStream extends stream.Readable {
    constructor(text, options) {
        super(options);
        this.text = text;
        this.eos = false;
    }
    _read() {
        if (!this.eos) {
            this.push(this.text);
            this.eos = true
        } else
            this.push(null);
    }
}

function write(text) {
    out = out + text;
}

function indent(level) {
    while (level--)
        write(' ');
}

function callback(token, tag, attribute, value) {
    switch (token) {
        case xppq.tokens.ERROR:
            process.stdout.write(">>> ERROR  :'" + value + "'\n");
            break;
        case xppq.tokens.DONE:
            process.stdout.write(out);
            break;
        case xppq.tokens.START_TAG:
            indent(indentLevel);
            write("Start tag :'" + tag + "'\n");
            indentLevel++;
            break;
        case xppq.tokens.ATTRIBUTE:
            indent(indentLevel);
            write("Attribute :'" + attribute + "' = '" + value + "'\n");
            break;
        case xppq.tokens.VALUE:
            indent(indentLevel);
            write("Value     :'" + value.trim() + "'\n");
            break;
        case xppq.tokens.END_TAG:
            indentLevel--;
            indent(indentLevel);
            write("End tag   :'" + tag + "'\n");
            break;
        default:
            throw ("Unknown token !!!");
            break;
    }
}

const file = __dirname + '/demo.xml';
var id = 4;   // Default test id.
var arg = process.argv[2];

if (arg != undefined)
    id = Number(arg);

function getStream() {
    if ( true )
        return new StringStream(xml);
    else
        return fs.createReadStream(file);
}

console.log( xppq.componentInfo() );
console.log( xppq.wrapperInfo());
console.log( xppq.returnArgument('Basic test : this text comes from the addon (native code), and is written from Javascript.' ) );
console.log('     ---------------');

function test( id ) {
    switch ( id ) {
        case 0:
            console.log("No treatment ; to see the original file.\n");
            getStream().pipe(process.stdout);
            break;
        case 1:
            console.log("Piping the preprocessing stream.\n");
            new xppq.Stream(getStream()).on('error', (err) => console.error('\n>>> ERROR : ' + err + '\n')).pipe(process.stdout);
            break;
        case 2:
            console.log("Using the preprocessing stream with a callback, which transforms to lower case.\n");
            new xppq.Stream(getStream()).on('data', (chunk) => write(chunk.toString().toLowerCase())).on('error', (err) => console.error('\n>>> ERROR : ' + err + '\n')).on('end', () => console.log(out));
            break;
        case 3:
            console.log("XML parsing WITHOUT preprocessing.\n");
            xppq.parse(getStream(), callback);
            break;
        case 4:
            console.log("XML parsing WITH preprocessing.\n");
            xppq.parse(new xppq.Stream(getStream()).on('error', (err) => console.error('>>> ERROR : ' + err)), callback);
            break;
        default:
            console.error("'" + arg + "' is not a valid test id ; must be '0' to '4'.");
            break;
    }
}

test( id );
