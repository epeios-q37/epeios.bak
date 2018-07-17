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
const types = require('./XDHqSHRD.js').types;

function getResponse(query, type) {
  switch (type) {
    case types.UNDEFINED:
      throw "This function should not be called with UNDEFINED type !!!";
      break;
    case types.VOID:
      throw "The VOID type should be handled upstream !!!";
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

function launch(createCallback, newSessionAction, callbacks) {
	if (process.env.EPEIOS_SRC) {
		console.log("DEMO mode !");
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

        c._xdhDOM = createCallback(c);
        c._xdhDOM._xdhSocket = c;

        c.write(Buffer.from("StandBy_1\x00"));
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
            callbacks[newSessionAction](c._xdhDOM, "");
            //            c.write(Buffer.from("StandBy_1\x00"));
          } else {
            callbacks[action](c._xdhDOM, id);
          }
        } else {
          console.log("READY !!!", c._xdhDOM._xdhType);
          if (c._xdhDOM._xdhType === types.VOID) {
            if (c._xdhDOM._xdhCallback != undefined) {
              c._xdhDOM._xdhType = types.UNDEFINED;
              c._xdhDOM._xdhCallback();
              if (c._xdhDOM._xdhType === types.UNDEFINED)
                c.write(Buffer.from("StandBy_1\x00"));
            } else
              c.write(Buffer.from("StandBy_1\x00"));
          } else if (c._xdhDOM._xdhCallback != undefined) {
            var type = c._xdhDOM._xdhType;
            c._xdhDOM._xdhType = types.UNDEFINED;
            c._xdhDOM._xdhCallback(getResponse(query, type));
            if (c._xdhDOM._xdhType === types.UNDEFINED)
              c.write(Buffer.from("StandBy_1\x00"));
          } else {
            getResponse(query, c._xdhDOM._xdhType);
            c.write(Buffer.from("StandBy_1\x00"));
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

function call( dom, command, type ) {
  var i = 3;
  var data = Buffer.from(command + '\x00');
  var amount = arguments[i++];

  dom._xdhType = type;

  while ( amount-- )
    data = add(data, arguments[i++]);

  amount = arguments[i++];

  while ( amount-- )
    data = add(data, arguments[i++]);

  dom._xdhCallback = arguments[i++];

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

module.exports.launch = launch;
module.exports.call = call;
