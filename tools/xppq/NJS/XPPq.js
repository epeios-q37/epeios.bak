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
    cb(null,  njsq._call(xppq, 21, this, chunk));
  }

  _flush(cb) {
    cb(null, njsq._call(xppq, 22, this));
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
module.exports.parse = (flow, callback) => { overload(modes.READABLE, flow, 10, 11); njsq._call(xppq, 12, flow, callback) };
module.exports.tokens = tokens;
