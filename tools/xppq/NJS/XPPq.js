/*
	Copyright (C) 2007 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq software.

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

"use strict"

var affix = "xppq";

// Begin of generic part.
var njsq = null;
var componentPath = null;
var componentFilename = null;
var path = require("path");

if (process.env.Q37_EPEIOS) {
  if (process.platform == 'win32')
    componentPath = 'h:/bin/';
  else
    componentPath = '/home/csimon/bin/';
  njsq = require(componentPath + 'njsq.node');
} else {
  njsq = require('njsq');
  componentPath = __dirname;
}

componentFilename = path.join(componentPath, "lib" + affix + "njs").replace(/\\/g, "\\\\").replace(/'/g, "\\'").replace(/ /g, "\\ ");
const xppq = njsq._register(componentFilename);
// End of generic part.

module.exports.returnArgument = (text) => { return njsq._call(xppq, 0, text) };

const stream = require('stream');

function onReadable(flow, onData, onEnd) {
  var chunk;

  /*    if ( ( chunk = flow.read() ) != null )
      do njsq._call(xppq,onRead, flow, chunk);
      while ( ( chunk = flow.read() ) != null);
    else
      njsq._call(xppq,onEnd, flow);
  */

  if ((chunk = flow.read()) != null)
    njsq._call(xppq, onData, flow, chunk);
  else
    njsq._call(xppq, onEnd, flow);
}

var modes = {
  READABLE: 0,
  DATA_END: 1
};

function overload(mode, flow, onData, onEnd) {
//  flow.setEncoding('utf8');
  if (mode == modes.READABLE)
    flow.on('readable', () => onReadable(flow, onData, onEnd));
  else if (mode == modes.DATA_END) {
    flow.on('data', (chunk) => {njsq._call(xppq, onData, flow, chunk); process.stdout.write(""); });
    // The 'process.stdout.write("")' will do nothing, but the parser hangs
    // when parsing the preprocessor output if missing.
    flow.on('end', () => {njsq._call(xppq, onEnd, flow) } );
  } else
    throw new Error("Unknown mode...");
}

// For debugging.
class PassthroughStream extends stream.Transform {
  _transform(chunk, enc, cb) {
//    var upperChunk = chunk.toString().toUpperCase();
//    this.push(upperChunk);
    cb(null,  njsq._call(xppq, 40, chunk));
  }

  constructor(options) {
    super(options);
  }
}

class Stream extends stream.Transform {
  constructor(options) {
    super(options);
    njsq._call(xppq, 20, this)
  }
  _transform(chunk, enc, cb) {
    let data = njsq._call(xppq, 21, this, chunk);

    if ( data === null )
      cb(njsq._call(xppq, 23, this));
    else
      cb(null, data);
  }

  _flush(cb) {
    let data = njsq._call(xppq, 22, this);

    if ( data === null )
      cb(njsq._call(xppq, 23, this));
    else
      cb(null, data);
  }
}

class DummyStream_ extends stream.Readable {
  constructor(options) {
    super(options);
    njsq._call(xppq, 15, this);
  }
  _read() {
    console.log("Before");
    if (!this.eos) {
      njsq._call(xppq, 14, this);
      this.eos = true
    } else
      this.push(null);
    console.log("After");
  }
}

class DummyStream extends stream.Readable {
  constructor(options) {
    super(options);
  }
  _read() {
    console.log("Before");
    if (!this.eos) {
      this.push("Year!");
      this.eos = true
    } else
      this.push(null);
    console.log("After");
  }
}

// If modified, modify also 'parser.cpp'.
var tokens = {
  ERROR: 0,
  DONE: 1,
  START_TAG: 2,
  ATTRIBUTE: 3,
  VALUE: 4,
  END_TAG: 5
};

module.exports.componentInfo = () => njsq._componentInfo(xppq);
module.exports.wrapperInfo = () => njsq._wrapperInfo();
module.exports.Stream = Stream;
module.exports.PassthroughStream = PassthroughStream;
module.exports.DummyStream = DummyStream_;
module.exports.basic = (flow) => { overload(modes.DATA_END, flow, 30, 31) };
module.exports.parse = (flow, callback, userObject) => { overload(modes.READABLE, flow, 10, 11); njsq._call(xppq, 12, flow, callback, userObject  == undefined ? new Object : userObject) };
module.exports.tokens = tokens;
