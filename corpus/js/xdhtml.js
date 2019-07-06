/*
	Copyright (C) 2014 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

const onEventAttributeName = "data-xdh-onevent";
const onEventsAttributeName = "data-xdh-onevents";
const widgetAttributeName = "data-xdh-widget";
const valueAttributeName = "data-xdh-value";
const styleId = "XDHStyle";

var counter = 0;
var drag = false;

function getElement(elementOrId) {
	if (typeof elementOrId === "string")
		if (elementOrId === "")
			return document.body.firstElementChild;
		else
			return document.getElementById(elementOrId);
	else if (elementOrId === document.body)	// To avoid defacement.
		return getElement("");
	else
		return elementOrId;
}

function getOrGenerateId(element) {
	if (element === null)
		return "";

	if (!element.id)
		element.id = "_CGN" + String(counter++);

	return element.id;
}

// Returns for the 'INPUT' tags the content of the 'type' attribute instead of 'INPUT'.
function getPatchedNodeName(node) {
	let name = node.nodeName;	// Should be always uppercased.

	if (name === 'INPUT') {
		let type = node.type;

		if (type)
			return type.toUpperCase();
		else
			return "TEXT";
	} else
		return name;
}

function parseXML(string) {
	var parser = new DOMParser();

	return parser.parseFromString(string, "text/xml");
}

function getStylesheet(xslName) {
	var xsltProcessor = new XSLTProcessor();

	//	console.log(xslName);

	if (false ) {
		xsltProcessor.importStylesheet(parseXML(xslName));
	} else {
		let myXMLHTTPRequest = new XMLHttpRequest();
		myXMLHTTPRequest.open("GET", xslName, false);
		myXMLHTTPRequest.send(null);

		xsltProcessor.importStylesheet(parseXML(myXMLHTTPRequest.responseText));
	}

	return xsltProcessor;
}

function getNULString(data, offset) {
	let string = "";
	let c = data.substr(offset++, 1);

	while (c !== '\0') {
		string += c;
		c = data.substr(offset++, 1);
	}

	return [string, offset];
}

function getString(data, offset) {
	return getNULString(data, offset);
}

// Not used...
function xmls(str) {
	str = String(str).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;').replace(/'/g, '&apos;');

	return str;
}

function convert(xml) {
	let offset = 0;
	let tag = "";
	let name = "";
	let value = "";
	let length = xml.length;

	[name, offset] = getNULString(xml, offset);	// Currently ignored.

	[name, offset] = getString(xml, offset);

	let noRoot = name === "";

	if (noRoot)
		name = "dummy";

	let rootNode = document.implementation.createDocument(null, name).firstChild;

	let node = rootNode;

	while (offset < length) {
		switch (xml[offset++]) {
		case '>':	// Push tag.
			[name, offset] = getString(xml, offset);
			node = node.appendChild(node.ownerDocument.createElement(name));
			break;
		case '<':	//Pop tag.
			node = node.parentNode;
			break;
		case 'A':	// Set attribute
			[name, offset] = getString(xml, offset);
			[value, offset] = getString(xml, offset);
			node.setAttribute(name, value );
			break;
		case 'V':	// Set value.
			[value,offset] = getString( xml, offset );
			node.textContent = value;
			break;
		default:
			throw "Error !";
			break;
		}
	}

	node = rootNode;

	if (noRoot)
		node = node.firstChild;

	let result = "";

	while (node !== null) {
		log(node);
		result += new XMLSerializer().serializeToString(node);
		node = node.nextSibling;
	}

	log(result);

	return result;
}

function transformToFragment(xml, xslName) {
	return getStylesheet(xslName).transformToFragment(parseXML(xml), document);
}

function transformToDocument(xml, xslName) {
	return getStylesheet(xslName).transformToDocument(parseXML(xml)).documentElement;
}

function transformToText(xml, xslName) {
	return transformToDocument(xml, xslName).textContent;
}

function transformToHTML(xml, xslName) {
	return transformToDocument(xml, xslName).innerHTML;
}

function removeChildren(elementOrId) {
	getElement(elementOrId).innerHTML = "";
}

function getLayoutHTML(xml, xsl) {
	if (xml.trim().substring(0, 1) !== "<")
		xml = convert(xml);

	if (xsl === "") {
		return xml;
	} else {
		let div = document.createElement('div');
		div.appendChild(transformToFragment(xml, xsl));
		return div.innerHTML;
	}
}

function prependLayout(id, xml, xsl) {
	element = getElement(id);

	element.innerHTML = getLayoutHTML(xml, xsl) + element.innerHTML;
}

function setLayout(id, xml, xsl) {
	getElement(id).innerHTML = getLayoutHTML(xml, xsl);
}

function appendLayout(id, xml, xsl) {
	element = getElement(id);

	element.innerHTML = element.innerHTML + getLayoutHTML(xml, xsl);
}

function handleBooleanAttribute(element, name, flag) {
	if (flag)
		element.setAttribute(name, name);
	else
		element.removeAttribute(name);
}

function patchATags(node)  // Patches the 'A' tags, so it does open in another browser Windows (when using 'xdhwebq').
{
	var cont = true;
	var root = node;
	var candidate;

	if (node.firstChild === null)
		cont = false;

	while (cont) {
		if (node.nodeType === Node.ELEMENT_NODE) {
			if (node.nodeName === "A")
				patchATag(node);
		}

		if ((candidate = node.firstChild) === null) {
			while (cont
				&& (candidate = node.nextSibling) === null) {
				node = node.parentNode;

				if (node.isEqualNode(root))
					cont = false;
			}
		}

		node = candidate;
	}
}

function setContent(doc, id, content) {
	var element = doc.getElementById(id);

	if (element !== null) {
		var tagName = element.tagName;

		switch (tagName) {
			case "INPUT":
				element.value = content;
				break;
			case "SPAN":
				element.innerHTML = content;
				break;
			case "TEXTAREA":
				element.value = content;
				break;
			default:
				element.innerHTML = content;
				// throw tagName + ": content setting not handled !";
				break;
		}

		patchATags(element);
	}
}

function setContents(ids, contents) {
	var i = ids.length;

	if (ids.length !== contents.length)
		throw "Inconsistency";

	while (i--) {
		setContent(document, ids[i], contents[i]);
	}
}

function patchATag(node) {
    if (node.nodeName !== "A")
        throw "Not a 'A' tag !!!";

	if (!node.hasAttribute("target")) {
		node.setAttribute("target", "_blank");	// So the app isn't overwritten by the content of the link...
		node.setAttribute("onclick", "event.stopPropagation();");	// To prevent edition of the element when clicking a link.
	}
}

function fetchEventHandlers(id) {
	var root = getElement(id);
	var node = root;	// Handling root, due to 'About'/'Refresh' corresponding action handling.
	var cont = true;
	var candidate;
	var digests = "";

	if (node.firstChild === null)
		cont = false;

	while (cont) {
		if (node.nodeType === Node.ELEMENT_NODE) {
			if (node.hasAttribute(onEventAttributeName))
				digests += "(" + getOrGenerateId(node) + "|" + getPatchedNodeName( node ) + "|((" + node.getAttribute(onEventAttributeName) + ")))|";

			if (node.hasAttribute(onEventsAttributeName))
				digests += "(" + getOrGenerateId(node) + "|" + getPatchedNodeName(node) + "|(" + node.getAttribute(onEventsAttributeName) + "))|";

			if (node.nodeName === "A")
				patchATag(node);
		}

		if ((candidate = node.firstChild) === null) {
			while (cont
				&& (candidate = node.nextSibling) === null) {
				node = node.parentNode;

				if (node.isEqualNode(root))
					cont = false;
			}
		}

		node = candidate;
	}

	if (typeof convertTrees === 'function')
		convertTrees();	// from 'mktree'.

	return digests;
}

function fetchWidgets(id) {
	var root = getElement(id);
	var node = root.firstChild;
	var cont = true;
	var candidate;
	var digests = "";

	if (node === null)
		cont = false;

	while (cont) {
		if (node.nodeType === Node.ELEMENT_NODE) {
			if (node.hasAttribute(widgetAttributeName))
				digests += "(" + getOrGenerateId(node) + "|(" + node.getAttribute(widgetAttributeName) + "))|";
		}

		if ((candidate = node.firstChild) === null) {
			while (cont && (candidate = node.nextSibling) === null) {
				node = node.parentNode;

				if (node.isEqualNode(root))
					cont = false;
			}
		}

		node = candidate;
	}

	return digests;
}

function getValue(elementOrId)	// Returns the value of element of id 'id'.
{
	var element = getElement(elementOrId);
	var tagName = element.tagName;

//	console.log("VALUE:", element.textContent);

	switch (tagName) {
		case "INPUT":
			switch (element.getAttribute("type")) {
				case "checkbox":
				case "radio":
					return element.checked;
					break;
				default:
					return element.value;
					break;
			}
			break;
		case "TEXTAREA":
			return element.value;
			break;
		case "SELECT":
			if (element.selectedIndex == -1)
				return "";
			else
				return element.options[element.selectedIndex].value;
			break;
		case "OPTION":
			return element.value;
			break;
		case "text":	// SVG
		case "tspan":	// SVG
			return element.textContent;
			break;
		default:
			return element.getAttribute(valueAttributeName);
			break;
	}
}

function setValue(id, value) {
	var element = document.getElementById(id);
	var tagName = element.tagName;

	switch (tagName) {
		case "INPUT":
			element.value = value;
			break;
		case "TEXTAREA":
			element.innerHTML = value;
			break;
		default:
			throw tagName + ": value setting not handled !";
			break;
	}
}

function setEventHandlers(ids, events) {
	var i = ids.length;

	if (ids.length !== events.length)
		throw "Inconsistency !";

	while (i--) {
		document.getElementById(ids[i]).addEventListener(events[i], handleEvent, false);
		//		jQuery( document.getElementById( ids[i] ) ).on( "select_node.jstree", function (e, data) {  alert( "toto" );} );
	}
}

function instantiateWidget(id, type, parameters) {
	var script = 'jQuery( document.getElementById( "' + id + '") ).' + type + '(' + parameters + ');';
	return script;
}

function instantiateWidgets(ids, types, parametersSets) {
	var i = ids.length;
	var script = "";

	if (ids.length !== types.length)
		throw "Inconsistency";

	if (ids.length !== parametersSets.length)
		throw "Inconsistency";

	while (i--) {
		script += instantiateWidget(ids[i], types[i], parametersSets[i]);
	}

	eval(script);
}

function buildDigest(target, type, keys) {
	var digest = "";

	if (target.hasAttribute("data-xdh-onevent"))
		digest += "(" + target.getAttribute("data-xdh-onevent") + ")";

	if (target.hasAttribute("data-xdh-onevents"))
		digest += target.getAttribute("data-xdh-onevents");

	if (!digest) {
		return digest;
	}

	if (digest && !target.id)
		throw "Event digest with no id !";

	digest = target.id + "|" + getPatchedNodeName(target) + "|" + type + "|" + keys + "|(" + digest + ")";

	return digest;
}

function handleEvent(event) {
	var currentTarget = event.currentTarget;	// Object which owns the event handler.
	var target = event.target;	// Object which launches the event.
	var keys = "";
	var digest = "";
	var message = "";

	event.stopPropagation();

	if (event.type === "dragstart") {
		event.dataTransfer.clearData();
		event.dataTransfer.setData("Untyped", "Dummy");	// Otherwise drag & drop doesn't work with Firefox.
	}

	if (event.type === "drop")
		event.preventDefault();

	if (event.type.search("key") === 0) {
		if (event.ctrlKey)
			keys += "C";

		if (event.shiftKey)
			keys += "S";

		if (event.altKey)
			keys += "A";

		if (event.metaKey)
			keys += "M";

		if (keys.length > 0)
			keys += '+';

		var key = event.which || event.keyCode;

		if (key === 13) {
			keys += "Enter"
			event.preventDefault();	/* Without this, when a text box has an 'Enter' event handler, the 'Enter' will be also applied to a
									   to a potential alert box reporting an error, preventing it to be displayed. This is only a workaround ;
									   another solution should be found. */
		} else if (key === 27)
			keys += "Esc"
		else if (key < 32)
			key += 96;

		if (key >= 32) {
			key = String.fromCharCode(key)

			if (key === '(' || key === '|' || key === '\\' || key === ')')	// This keys are used as separators, or escape char ('\'), by 'strmrg' module, so they must be escaped.
				keys += '\\';

			keys += key.toLowerCase();
		}
	}

	message = "----------> Detected event : '" + event.type + "' ";

	if (keys)
		message += "'" + keys + "' ";

	message += "on '" + currentTarget.id + "' (" + getPatchedNodeName(currentTarget) + ").";

	log(message);

	digest = buildDigest(event.currentTarget, event.type, keys);

	if (!digest) {
		log("Event CANCELLED !!!");
		return;
	}

	if (launchEvent(digest))
		event.stopPropagation();
}

function mktreeExpandToNode(element) {
	var tree = element;

	while (tree && tree.className !== "mktree")
		tree = tree.parentNode;

	if (tree) {
		while (element && element.tagName !== "LI")
			element = element.parentNode;

		if (element)
			expandToItem(getOrGenerateId(tree), getOrGenerateId(element));
	}
}

function getCSSRules(id) {
	if (id === "")
		id = styleId;

	return document.getElementById(id).sheet;
}

function insertCSSRule(id, rule, index) {
	var rules = getCSSRules(id);

	if (index === -1)
		index = rules.cssRules.length;

	rules.insertRule(rule, index);

//	console.log(index);

	return index;
}

function removeCSSRule(id, index) {
//	console.log(getCSSRules().cssRules.length + " : " + index);
	getCSSRules(id).removeRule(index);
}

function handleClasses(ids, classes, method) {
	var i = 0;

	if (ids.length !== classes.length)
		throw "Inconsistency !";

	while (i < ids.length) {
		method(ids[i], classes[i]);

		i++;
	}
}

function addClasses(ids, classes) {
	handleClasses(ids, classes, (id, clas) => document.getElementById(id).classList.add(clas));
}

function removeClasses(ids, classes) {
	handleClasses(ids, classes, (id, clas) => document.getElementById(id).classList.remove(clas));
}

function toggleClasses(ids, classes) {
	handleClasses(ids, classes, (id, clas) => document.getElementById(id).classList.toggle(clas));
}

function enableElements(ids) {
	var i = 0;

	while (i < ids.length) {
		document.getElementById(ids[i]).disabled = false;

		i++;
	}
}

function disableElements(ids) {
	var i = 0;

	while (i < ids.length) {
		document.getElementById(ids[i]).disabled = true;

		i++;
	}
}
