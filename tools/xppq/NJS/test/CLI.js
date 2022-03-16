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

"use strict"

const xml = `
<?xml version="1.0" encoding="UTF-8"?>
<SomeTag xmlns:xpp="http://q37.info/ns/xpp/" AnAttribute="SomeAttributeValue">
 <SomeOtherTag AnotherAttribute="AnotherAttributeValue">TagValue (in a string)</SomeOtherTag>
 <xpp:define name="SomeMacro">
  <xpp:bloc>Some macro content !</xpp:bloc>
 </xpp:define>
 <YetAnotherTag YetAnotherAttribute="YetAnotherAttributeValue">
  <xpp:expand select="SomeMacro"/>
 </YetAnotherTag>
</SomeTag>
`;

const fs = require('fs');
const stream = require('stream');
const { getSystemErrorMap } = require('util');
const xppq = require('./XPPq.js');
var indentLevel = 0;

var out = "";
var fileName = "";

class StringStream extends stream.Readable {
  constructor(text, options) {
    super(options);
    this.text = text;
  }
  _read() {
    if (!this.eos) {
      this.push(this.text);
      this.eos = true
    } else {
      this.push(null);
    }
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
      throw new Error("ERROR :'" + value + "'\n");
      break;
    case xppq.tokens.DONE:
      process.stdout.write(out);
      break;
    case xppq.tokens.START_TAG:
      write("Start tag:");
      indent(indentLevel);
      write("'" + tag + "'\n");
      indentLevel++;
      break;
    case xppq.tokens.ATTRIBUTE:
      write("Attribute:");
      indent(indentLevel);
      write("'" + attribute + "' = '" + value + "'\n");
      break;
    case xppq.tokens.VALUE:
      write("Value    :");
      indent(indentLevel);
      write("'" + value.trim() + "'\n");
      break;
    case xppq.tokens.END_TAG:
      indentLevel--;
      break;
    default:
      throw new Error("Unknown token !!!");
      break;
  }
}

var id = 0;

switch ( process.argv.length ) {
  case 4:
    fileName = process.argv[3];
    // fall through
  case 3:
    id = Number(process.argv[2]);
    break;
  default:
    process.stdout.write("Usage: node CLI.js 0…3 [XML file name]\n");
    process.exit();
    break;
}
    
function getStream() {
  return fileName === "" ? new StringStream(xml) : fs.createReadStream(fileName);
//    return fs.createReadStream(__dirname + '/Project.xml');
    
}

process.stdout.write(xppq.componentInfo());
process.stdout.write('\n');
process.stdout.write(xppq.wrapperInfo());
process.stdout.write('\n');
process.stdout.write(xppq.returnArgument('Basic test - this text comes from the addon (native code), and is written from Javascript\n'));
process.stdout.write('\n     ---------------\n');

switch (id) {
  case 0:
    process.stdout.write("No treatment; to see the original XML data.\n");
    getStream().pipe(process.stdout);
    break;
  case 1:
    process.stdout.write("Preprocessing XML data.\n");
    getStream().pipe(new xppq.Stream).pipe(process.stdout);
    break;
  case 2:
    process.stdout.write("XML data parsing WITHOUT preprocessing.\n");
    xppq.parse(getStream(), callback);
    break;
  case 3:
    process.stdout.write("XML data parsing WITH preprocessing.\n");
    xppq.parse(getStream().pipe(new xppq.Stream), callback);
    break;
  default:
    process.stderr.write("'" + id + "' is not a valid id; must be '0' to '3'.\n");
}
