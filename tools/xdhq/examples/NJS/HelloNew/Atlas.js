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

const path = require('path');

var xdhqId = "";
var xdhwebqId = "";
var xdhelcqPath = "";
var xdhelcqBin = "";
var electronBin = "";

function isDev() {
  if (process.env.EPEIOS_SRC)
    return true;
  else
    return false;
}

function getEpeiosPath() {
  if (isDev) {
    if (process.platform == 'win32') {
      return "h:/hg/epeios/"
    } else {
      return "~/hg/epeios/"
    }
  } else
    throw "Error !";
}

function getRealDir(dir) {
  if (isDev()) {
    let epeiosPath = getEpeiosPath();
    return path.resolve(epeiosPath, "tools/xdhq/examples/common/", path.relative(path.resolve(epeiosPath, "tools/xdhq/examples/NJS/"), path.resolve(dir)));	// No final '/'.
  } else
    return path.resolve(dir);
  getsize
}

if (isDev()) {
  let epeiosToolsPath = "";
  let binPath = "";
  if (process.platform == 'win32') {
    epeiosToolsPath = "h:/hg/epeios/tools/";
    binPath = "h:/bin/";
  } else {
    epeiosToolsPath = "~/hg/epeios/tools/";
    binPath = ~/bin/
  }

  xdhqId = epeiosToolsPath + "xdhq/wrappers/NJS/XDHq.js";
  xdhwebqId = epeiosToolsPath + "xdhwebq/NJS/XDHWebQ.js";
  xdhelcqPath = epeiosToolsPath + "xdhelcq/";
  xdhelcqBin = path.join(binPath, "xdhqxdh");
  electronBin = xdhelcqPath + "node_modules/electron/dist/electron";
} else {
  xdhqId = "xdhqnjs";
  xdhwebqId = "xdhwebq";
  xdhelcqPath = path.dirname(require.resolve("xdhelcq"));
  xdhelcqBin = require('xdhqxdh').fileName;
  electronBin = require("xdhelcq").electron;
}

const xdhq = require(xdhqId);

function launchWeb(dir) {
  require('child_process').fork(require(xdhwebqId).fileName, [dir]);
}

function launchDesktop(dir) {
  require('child_process').spawn(electronBin, [path.join(xdhelcqPath, "index.js"), "-s=localhost:53752", "-m=" + xdhelcqBin, dir]).on('close', function (code) {
    process.exit(code)
  });
}

const guis = {
  NONE: 0,
  DESKTOP: 1,
  WEB: 2,
  DESKTOP_AND_WEB: 3
}

module.exports.guis = guis;

const defaultGUI = guis.DESKTOP;

function getSize(query, offset) {
  var size = query[offset];

  return size;
}

function getString(query, offset) {
  var size = getSize(query, offset++);
  var string = "";

  while (size--)
    string += String.fromCodePoint(query[offset++]);

  return string;
}

function getStrings(query, offset) {
  var size = getSize(query, offset++);

  var strings = new Array();

  while (size--) {
    strings.push(getString(query, offset));
    offset += getSize(query, offset) + 1;
  }

  return strings;
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

function getQuery(socket, action, id) {
  var buffer;
  var query = Buffer.alloc(0);

  while (buffer = socket.read())
    query = Buffer.concat([query, buffer]);

  return query;
}

function getId(query) {
  return getString(query, 9);
}

function getAction(query) {
  return getString(query, 9 + getSize(query, 9) + 1);
}

// Types of the reponse.
const types = {
  UNDEFINED: 0, // No pending response.
  NONE: 1,
  STRING: 2,
  STRINGS: 3
}

function getResponse(query, type) {
  switch (type) {
    case types.UNDEFINED:
      throw "This function should not be called with UNDEFINED type !!!";
      break;
    case types.NONE:
      throw "The NONE type should be handled upstream !!!";
      break;
    case types.STRING:
      return getString(query, 8);
      break;
    case types.STRINGS:
      return getStrings(query, 8);
      break;
    default:
      throw "Unknown response type !!!";
      break;
  }
}

function launch(create, callback, callbacks, gui) {
  var dir = getRealDir(path.dirname(process.argv[1]));

  if (gui === undefined) {
    if (process.argv.length > 2) {
      switch (process.argv[2]) {
        case "n":
        case "none":
          gui = guis.NONE;
          break;
        case "d":
        case "desktop":
          gui = guis.DESKTOP;
          break;
        case "web":
        case "w":
          gui = guis.WEB;
          break;
        case "dw":
        case "wd":
          gui = guis.DESKTOP_AND_WEB;
          break;
        default:
          throw ("Unknown gui !");
          break;
      }
    } else
      gui = defaultGUI;
  }

  //	xdhq.launch(callback, action);
  const server = net.createServer((c) => {
    // 'connection' listener
    //		c.on('data', function(chunk) {console.log( ">" + chunk.toString() + "<" ) });
    c.on('readable', () => {

      if (c._xdhDOM === undefined) {
        var data;

        while (data = c.read())
          console.log(">" + data + ' ||| ' + Buffer.from(data) + "<");

        c._xdhDOM = create(c);
        c._xdhCallbacks = callbacks;
        c._xdhDOM._xdhSocket = c;

        c.write(Buffer.from("OK_1\x00"));
      } else {
        var query;

        query = getQuery(c);

        console.log("Query: ", query.toString());

        if (hasToLaunch(query)) {
          var id, action;

          id = getId(query);
          action = getAction(query);
          console.log("action: '" + action + "', id: '" + id + "'");

          if (action == "") {
            callback(c._xdhDOM, "");
            //            c.write(Buffer.from("OK_1\x00"));
          } else {
            c._xdhCallbacks[action](c._xdhDOM, id);
            //            c.write(Buffer.from("OK_1\x00"));
          }
        } else {
          console.log("READY !!!", c._xdhDOM._xdhType);
          if (c._xdhDOM._xdhType === types.NONE) {
            if (c._xdhDOM._xdhCallback != undefined) {
              c._xdhDOM._xdhType = types.UNDEFINED;
              c._xdhDOM._xdhCallback();
              if (c._xdhDOM._xdhType === types.UNDEFINED)
                c.write(Buffer.from("OK_1\x00"));
            } else
              c.write(Buffer.from("OK_1\x00"));
          } else if (c._xdhDOM._xdhCallback != undefined) {
            var type = c._xdhDOM._xdhType;
            c._xdhDOM._xdhType = types.UNDEFINED;
            c._xdhDOM._xdhCallback(getResponse(query, type));
            if (c._xdhDOM._xdhType === types.UNDEFINED)
              c.write(Buffer.from("OK_1\x00"));
          } else {
            getResponse(query, c._xdhDOM._xdhType);
            c.write(Buffer.from("OK_1\x00"));
          }
        }
      }
    });

    console.log('client connected');
  });
  server.on('error', (err) => {
    throw err;
  });
  server.listen(53752, () => {
    console.log('server bound');
  });


  switch (gui) {
    case guis.NONE:
      break;
    case guis.DESKTOP:
      launchDesktop(dir);
      break;
    case guis.WEB:
      launchWeb(dir);
      break;
    case guis.DESKTOP_AND_WEB:
      launchDesktop(dir);
      launchWeb(dir);
      break;
    default:
      throw ("Unknown gui !");
      break;
  }
}

const net = require('net');

function addString(data, string) {
  return Buffer.concat([data, Buffer.alloc(1, string.length), Buffer.from(string)]);
}

function addStrings(data, strings) {
  var i = 0;
  data = Buffer.concat([data, Buffer.alloc(1, strings.length)]);

  while (i < strings.length)
    data = addString(data, strings[i++]);

  return data;
}

function add(data, argument) {
  if (typeof (argument) === "string")
    return addString(data, argument);
  else if (typeof (argument) === "object")
    return addStrings(data, argument);
  else
    throw "Unexpected argument type: " + typeof (argument);
}

function call(dom, command, type, callback) {
  // console.log( arguments.length, arguments );

  var i = 4;
  var data = Buffer.from(command + '\x00');

  while (i < arguments.length) {
    console.log("argument[" + i + "](" + typeof (arguments[i]) + "): ", arguments[i]);
    data = add(data, arguments[i]);
    i++;
  }

  dom._xdhType = type;
  dom._xdhCallback = callback;

  console.log("Data: ", data.toString());
  dom._xdhSocket.write(data);
}

// {'a': b, 'c': d, 'e': f} -> ['a','c','e'] [b,d,f]
function split(keysAndValues, keys, values) {
  for (var prop in keysAndValues) {
    keys.push(prop);
    values.push(keysAndValues[prop]);
  }
}

// ['a', 'b', 'c'] ['d', 'e', 'f'] -> { 'a': 'd', 'b': 'e', 'c': 'f' }
function unsplit(keys, values) {
  var i = 0;
  var keysValues = {};

  while (i < keys.length) {
    keysValues[keys[i]] = values[i];
    i++;
  }

  return keysValues;
}

// 'key', value -> { 'key': value } 
function merge(key, value) {
  var keyValue = {};

  keyValue[key] = value;

  return keyValue;
}

class DOM {
  setLayout(id, tree, xslFilename, callback) {
    call(this, "SetLayout_1", types.NONE, callback, id, tree.end(), xslFilename);
  }
  getContents(ids, callback) {
    call(this, "GetContents_1", types.STRINGS, (contents) => callback(unsplit(ids, contents)), ids);
  }
  getContent(id, callback) {
    this.getContents([id], (result) => { callback(result[id]); });
  }
  setContents(idsAndContents, callback) {
    var ids = [];
    var contents = [];

    console.log("!!!!!", idsAndContents);

    split(idsAndContents, ids, contents);

    call(this, "SetContents_1", types.NONE, callback, ids, contents);
  }
  setContent(id, content, callback) {
    this.setContents(merge(id, content), callback);
  }
  confirm(message, callback) {
    call(this, "Confirm_1", types.STRING, (result) => callback(result == "true"), message);
  }
}

module.exports.register = xdhq.register;
module.exports.launch = launch;
module.exports.createTree = xdhq.createTree;
module.exports.DOM = DOM;
