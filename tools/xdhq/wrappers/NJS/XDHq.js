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
const fs = require('fs');
const path = require('path');
const shared = require('./XDHqSHRD.js');

const types = shared.types;
const platforms = shared.platforms;
const platform = shared.platform;
const open = shared.open;
const isDev = shared.isDev;

function getEpeiosPath() {
	if (isDev()) {
		if (platform === platforms.WIN32) {
            return "h:/hg/epeios/";
		} else {
            return fs.realpathSync(process.env.Q37_EPEIOS);
		}
	} else
		throw "Error !";
}

// Returns the directory which contains all the assets, based on the directory from where the app. were launched.
function getAssetDir() {
	var dir = path.dirname(process.argv[1]);

	if (isDev()) {
		let epeiosPath = getEpeiosPath();
		return path.resolve(epeiosPath, "tools/xdhq/examples/common/", path.relative(path.resolve(epeiosPath, "tools/xdhq/examples/NJS/"), path.resolve(dir)));	// No final '/'.
	} else {
		return path.resolve(dir);
    }
}

function getAssetFileName(fileName) {
    return path.join(getAssetDir(), path.win32.basename(fileName));
}

function readAsset(fileName) {
	return Buffer.from(fs.readFileSync(getAssetFileName(fileName))).toString();
}

/*
To allow the use of embedded XSL, if the first character begins with '<'
(as an XML declaration), the parameter is considered containing XSL,
otherwise the file name containing the XSL.
*/

function readXSLAsset(xslContentOrFilename) {
	if (xslContentOrFilename[0] === '<')
		return xslContentOrFilename;
	else
		return readAsset(xslContentOrFilename);
}

const modes = {
	FAAS: 0,
	SFLH: 1
};

// {'a': b, 'c': d, 'e': f} -> ['a','c','e'] [b,d,f]
function split(keysAndValues, keys, values) {
	for (var prop in keysAndValues) {
		keys.push(prop.toString());
		values.push(keysAndValues[prop].toString());
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

var xdhq;
var call;

function launch(callback, tagsAndCallbacks, head, mode) {
	switch (mode) {
		case modes.FAAS:
			xdhq = require('./XDHqFAAS.js');
			break;
		case modes.SLFH:
			xdhq = require('./XDHqSLFH.js');
			break;
		default:
			throw "Unknown mode !!!";
			break;
	}

	call = xdhq.call;
	xdhq.launch(callback, tagsAndCallbacks, head );
}

class XDH {
	execute_(type, script, callback, wait) {
		call(this, "Execute_1", type, script, callback, wait);
	}
	executeVoid(script, callback, wait) {
		this.execute_(types.VOID, script, callback, wait);
	}
	executeString(script, callback, wait) {
		this.execute_(types.STRING, script, callback, wait);
	}
	executeStrings(script, callback, wait) {
		this.execute_(types.STRINGS, script, callback, wait);
	}
	flush(id, callback, wait) {
		call(this, "Flush_1", types.STRING, id, callback, wait);
	}
	alert(message, callback, wait) {
        call(this, "Alert_1", types.STRING, message, callback, wait);
        // For the return value being 'STRING' instead of 'VOID',
        // see the 'alert' primitive in 'XDHqXDH'.
	}
	confirm(message, callback, wait) {
		call(this, "Confirm_1", types.STRING, message, callback === undefined ? callback : (answer) => callback(answer === "true"), wait);
	}
	handleLayout_(variant, id, xml, xsl, callback, wait) {
		if (typeof xml !== "string")
			xml = xml.toString();

		call(this, "HandleLayout_1", types.STRING, variant, id, xml, xsl, callback, wait);
	}
	prependLayout(id, html, callback, wait) {	// Deprecated!
		this.handleLayout_("Prepend", id, html, "", callback, wait);
	}
	appendLayout(id, html, callback, wait) {	// Deprecated!
		this.handleLayout_("Append", id, html, "", callback, wait);
	}
	handleLayoutXSL_(variant, id, xml, xslFilename, callback, wait) {
		let xslURL = xslFilename;

		if (this._xdh.isFAAS)
			xslURL = "data:text/xml;charset=utf-8," + encodeURIComponent(readXSLAsset(xslFilename));

		this.handleLayout_(variant, id, xml, xslURL, callback, wait);
	}
	prependLayoutXSL(id, xml, xsl, callback, wait) {	// Deprecated!
		this.handleLayoutXSL_("Prepend", id, xml, xsl, callback, wait);
	}
	setLayoutXSL(id, xml, xsl, callback, wait) {	// Deprecated!
		this.handleLayoutXSL_("Set", id, xml, xsl, callback, wait);
	}
	appendLayoutXSL(id, xml, xsl, callback, wait) {	// Deprecated!
		this.handleLayoutXSL_("Append", id, xml, xsl, callback, wait);
	}
	layout_(variant, id, xml, xslOrCallback, callbackOrWait, wait) {
		let xsl = "";
		let callback = undefined;

		if ( typeof xslOrCallback === "string") {
			xsl = xslOrCallback;
			callback = callbackOrWait;
		} else {
			callback = xslOrCallback;
			wait = callbackOrWait;
		}

		if ((xsl !== "") && this._xdh.isFAAS)
			xsl = "data:text/xml;charset=utf-8," + encodeURIComponent(readXSLAsset(xsl));

		call(this, "HandleLayout_1", types.STRING, variant, id, typeof xml === "string" ? xml : xml.toString(), xsl, callback, wait);
	}
	before(id,xml,xslOrCallback,callback, wait) {
		this.layout_("beforebegin", id, xml, xslOrCallback, callback, wait);
	}
	begin(id,xml,xslOrCallback,callback, wait) {
		this.layout_("afterbegin", id, xml, xslOrCallback, callback, wait);
	}
	inner(id,xml,xslOrCallback,callback, wait) {
		this.layout_("inner", id, xml, xslOrCallback, callback, wait);
	}
	end(id,xml,xslOrCallback,callback, wait) {
		this.layout_("beforeend", id, xml, xslOrCallback, callback, wait);
	}
	after(id,xml,xslOrCallback,callback, wait) {
		this.layout_("afterend", id, xml, xslOrCallback, callback, wait);
	}
	getContents(ids, callback, wait) {	// Deprecated
		call(this, "GetContents_1", types.STRINGS, ids, callback === undefined ? callback : (contents) => callback(unsplit(ids, contents), wait)
		);
	}
	getContent(id, callback, wait) {	// Deprecated
		return this.getContents([id], callback === undefined ? callback : (result) => { callback(result[id]); }, wait);
	}
	setContents(idsAndContents, callback, wait) {	// Deprecated
		var ids = [];
		var contents = [];

		split(idsAndContents, ids, contents);

		call(this, "SetContents_1", types.VOID, ids, contents, callback, wait);
	}
	setContent(id, content, callback, wait) {	// Deprecated
		return this.setContents(merge(id, content), callback, wait);
	}
	getValues(ids, callback, wait) {
		call(this, "GetValues_1", types.STRINGS, ids, callback === undefined ? callback : (values) => callback(unsplit(ids, values)), wait
		);
	}
	getValue(id, callback, wait) {
		return this.getValues([id], callback === undefined ? callback : (result) => { callback(result[id]); }, wait);
	}
	setValues(idsAndValues, callback, wait) {
		var ids = [];
		var values = [];

		split(idsAndValues, ids, values);

		call(this, "SetValues_1", types.VOID, ids, values, callback, wait);
	}
	setValue(id, value, callback, wait) {
		this.setValues(merge(id, value), callback, wait);
	}
	getMarks(ids, callback, wait) {
		call(this, "GetMarks_1", types.STRINGS, ids, callback === undefined ? callback : (marks) => callback(unsplit(ids, marks), wait)
		);
	}
	getMark(id, callback, wait) {
		return this.getMarks([id], callback === undefined ? callback : (result) => { callback(result[id]); }, wait);
	}
	setMarks(idsAndMarks, callback, wait) {
		var ids = [];
		var marks = [];

		split(idsAndMarks, ids, marks);

		call(this, "SetMarks_1", types.VOID, ids, marks, callback, wait);
	}
	setMark(id, mark, callback, wait) {
		return this.setMarks(merge(id, mark), callback, wait);
	}

		handleClasses(idsAndClasses, variant, callback, wait) {
		var ids = [];
		var classes = [];

		split(idsAndClasses, ids, classes);

		call(this, "HandleClasses_1", types.VOID, variant, ids, classes, callback, wait);
	}
	addClasses(idsAndClasses, callback, wait) {
		this.handleClasses(idsAndClasses, "Add", callback, wait);
	}
	addClass(id, clas, callback, wait) {
		this.addClasses(merge(id, clas), callback, wait);
	}
	removeClasses(idsAndClasses, callback, wait) {
		this.handleClasses(idsAndClasses, "Remove", callback, wait);
	}
	removeClass(id, clas, callback, wait) {
		this.removeClasses(merge(id, clas), callback, wait);
	}
	toggleClasses(idsAndClasses, callback, wait) {
		this.handleClasses(idsAndClasses, "Toggle", callback, wait);
	}
	toggleClass(id, clas, callback, wait) {
		this.toggleClasses(merge(id, clas), callback, wait);
	}
	enableElements(ids, callback, wait) {
		call(this, "EnableElements_1", types.VOID, ids, callback, wait);
	}
	enableElement(id, callback, wait) {
		this.enableElements([id], callback, wait);
	}
	disableElements(ids, callback, wait) {
		call(this, "DisableElements_1", types.VOID, ids, callback, wait);
	}
	disableElement(id, callback, wait) {
		this.disableElements([id], callback, wait);
	}
	setAttribute(id, name, value, callback, wait) {
		call(this, "SetAttribute_1", types.VOID, id, name, value, callback, wait);
	}
	getAttribute(id, name, callback, wait) {
		return call(this, "GetAttribute_1", types.STRING, id, name, callback, wait);
	}
	removeAttribute(id, name, callback, wait) {
		call(this, "RemoveAttribute_1", types.VOID, id, name, callback, wait);
	}
	setProperty(id, name, value, callback, wait) {
		call(this, "SetProperty_1", types.VOID, id, name, value, callback, wait);
	}
	getProperty(id, name, callback, wait) {
		return call(this, "GetProperty_1", types.STRING, id, name, callback, wait);
	}
	focus(id, callback, wait) {
		call(this, "Focus_1", types.VOID, id, callback, wait);
	}
	parent(id, callback, wait) {
		return call(this, "Parent_1", types.STRING, id, callback, wait);
	}
	firstChild(id, callback, wait) {
		return call(this, "FirstChild_1", types.STRING, id, callback, wait);
	}
	lastChild(id, callback, wait) {
		return call(this, "LastChild_1", types.STRING, id, callback, wait);
	}
	previousSibling(id, callback, wait) {
		return call(this, "PreviousSibling_1", types.STRING, id, callback, wait);
	}
	nextSibling(id, callback, wait) {
		return call(this, "NextSibling_1", types.STRING, id, callback, wait);
	}
	scrollTo(id, callback, wait) {
		call(this, "ScrollTo_1", types.VOID, id, callback, wait);
	}
	debugLog(switcher, callback, wait) {
		if ( switcher === undefined)
			switcher = true;
		else if ( typeof(switcher) !== "boolean" ) {
			if ( callback !== undefined)
				throw new Error("Bad parameter type!");
			callback = switcher;
			switcher = true;
		}

		call(this, "DebugLog_1", types.VOID, switcher ? "true" : "false" , callback, wait);
	}
	log(message, callback, wait) {
		call(this, "Log_1", types.VOID, message , callback, wait);
	}
}

module.exports.componentInfo = () => njsq._componentInfo(xdhq);
module.exports.wrapperInfo = () => njsq._wrapperInfo();
module.exports.returnArgument = (text) => { return njsq._call(xdhq, 0, text); };

module.exports.launch = launch;
module.exports.XDH = XDH;
module.exports.modes = modes;

module.exports.XML = require("./XDHqXML").XML;

// Following functions are dev helper.
module.exports.isDev = isDev;
module.exports.getAssetDir = getAssetDir;
module.exports.getAssetFileName = getAssetFileName;
module.exports.readAsset = readAsset;
module.exports.open = open;
module.exports.broadcastAction = (action, id) => xdhq.broadcastAction(action, id);
