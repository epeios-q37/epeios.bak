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
// 	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

const xdhURI = "http://q37.info/ns/xdh";
const xdhPrefix = "xdh:";
const xdhOldPrefix = "data-xdh-";

const xdhOnEvent = "onevent";
const xdhOnEvents = "onevents";
const xdhWidget = "widget";
const xdhRadio = "radio";
 // Method to retrieve widget content, but also if present, then the widget is already handled.
const xdhWidgetContentRetrievingMethod = "widget-content-retrieving-method";
const xdhContent = "content";	// Deprecated.
const xdhOldContent = "value";	// For compatibility with previous version.
const xdhMark = "mark"
const styleId = "XDHStyle";

function hasXDHAttribute(element, name) {
	return element.hasAttributeNS(xdhURI, name) || element.hasAttribute(xdhPrefix + name) || element.hasAttribute(xdhOldPrefix + name);
}

function getXDHAttribute(element, name) {
	return element.getAttributeNS(xdhURI, name) || element.getAttribute(xdhPrefix + name) || element.getAttribute(xdhOldPrefix + name);
}

function setXDHAttribute(element, name, value) {
	element.setAttribute(xdhPrefix + name, value);
}

var counter = 0;
var drag = false;

function getFrameDocument(frameId, doc) {
	let ifrm = doc.getElementById(frameId);

	return ifrm.contentDocument ? ifrm.contentDocument : ifrm.contentWindow.document;
}

function getDocument(id) {
	let iframes = id.split('@');

	if ( iframes.length === 1 )
		return document;
	else {
		let doc = document;
		let i = iframes.length - 1;

		while (i >= 1) { 
			doc = getFrameDocument(iframes[i], doc);
			i--;
		} 
	
		return doc;
	}
}

// Version handling 'iframes'.
/*
function getElement(elementOrId) {
	if (typeof elementOrId === "string") {
		let doc = getDocument(elementOrId);
		let id = elementOrId.split('@')[0];

		if (id === "")
			if ( doc === document )
				return doc.body.firstElementChild;
			else
				return doc.body;
		else
			return doc.getElementById(id);
	} else if (elementOrId === document.body)
		return getElement("");	// To avoid defacement.
	else
		return elementOrId;
}
*/

function getElement(elementOrId) {
	if (typeof elementOrId === "string") {
		if (elementOrId === "")
			return document.body.firstElementChild;
		else
			return document.getElementById(elementOrId);
	} else if (elementOrId === document.body)
		return getElement("");	// To avoid defacement.
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
			node.setAttribute(name, value);
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
	if ( xml.trim() === "" )
		return xml;
	else if (xml.trim().substring(0, 1) !== "<")
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
    getElement(id).insertAdjacentHTML("afterbegin", getLayoutHTML(xml, xsl));
}

function setLayout(id, xml, xsl) {
	getElement(id).innerHTML = getLayoutHTML(xml, xsl);
}

function appendLayout(id, xml, xsl) {
    getElement(id).insertAdjacentHTML("beforeend", getLayoutHTML(xml, xsl));
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

// Deprecated
function setContent(idOrElement, content) {
	var element = getElement(idOrElement);

	if (element !== null) {
		var tagName = element.tagName;

		switch (tagName) {
			case "INPUT":
				switch (element.getAttribute("type")) {
					case "checkbox":
					case "radio":
						element.checked = content.toLowerCase().trim() === 'true';
						break;
					default:
						element.value = content;
						break;
				}
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

function getCheckedRadio(element) {
	return document.querySelector('input[name="' + getXDHAttribute(element,  xdhRadio) + '"]:checked');
}

function setValue(idOrElement, value) {
	var element = getElement(idOrElement);

	if (element !== null) {
		var tagName = element.tagName;

		switch (tagName) {
			case "INPUT":
				switch (element.getAttribute("type")) {
					case "checkbox":
					case "radio":
						if ( element.hasAttribute("value") )
							element.value = value;
						else
							element.checked = value.toLowerCase().trim() === 'true';
						break;
					default:
						element.value = value;
						break;
				}
				break;
			case "TEXTAREA":
				element.value = value;
				break;
			case "SELECT":
				if ( value === "" )
					element.selectedIndex = -1;
				else {
					let option = element.querySelector('option[value="' + value + '"]');
					if ( option )
						element.selectedIndex = option.index;
				}
				break;
			default:
				if ( hasXDHAttribute( element, xdhRadio ) ) {
					if ( value === "" ) {
						let checked = getCheckedRadio(element);
						if ( checked )
							checked.checked = false;	
					} else {
						let radio = document.querySelector('input[name="' + getXDHAttribute(element,  xdhRadio) + '"][value="' + value + '"]');
						if ( radio )
							radio.checked = true;
					}
				}	else
					element.innerHTML = value;
				break;
		}

		patchATags(element);
	}
}

function setMark(idOrElement, mark) {
	setXDHAttribute(getElement(idOrElement), xdhMark, mark);
}

// Deprecated
function setContents(ids, contents) {	// Deprecated!
	let i = ids.length;

	if (ids.length !== contents.length)
		throw "Inconsistency";

	while (i--) {
		setContent(getElement(ids[i]), contents[i]);
	}
}

function setValues(ids, values) {
	let i = ids.length;

	if (ids.length !== values.length)
		throw "Inconsistency";

	while (i--) {
		setValue(ids[i], values[i]);
	}
}

function setMarks(ids, marks) {
	let i = ids.length;

	if (ids.length !== marks.length)
		throw "Inconsistency";

	while (i--) {
		setMark(ids[i], marks[i]);
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

function fetchEventHandlersAndWidgets(id) {
	var root = getElement(id);
	var node = root;	// Handling root, due to 'About'/'Refresh' corresponding action handling.
	var cont = true;
	var candidate;
	var eventDigests = "";
	var widgetDigests = "";

	if (node.firstChild === null)
		cont = false;

	while (cont) {
		if (node.nodeType === Node.ELEMENT_NODE) {
			if (hasXDHAttribute(node, xdhOnEvent))
				eventDigests += "(" + getOrGenerateId(node) + "|" + getPatchedNodeName( node ) + "|((" + getXDHAttribute(node, xdhOnEvent) + ")))|";

			if (hasXDHAttribute(node, xdhOnEvents))
				eventDigests += "(" + getOrGenerateId(node) + "|" + getPatchedNodeName(node) + "|(" + getXDHAttribute(node, xdhOnEvents) + "))|";
          
			if (hasXDHAttribute(node, xdhWidget))
				widgetDigests += "(" + getOrGenerateId(node) + "|(" + getXDHAttribute(node, xdhWidget) + "))|";


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

	return "(" + eventDigests + ")|(" + widgetDigests + ")";
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
			if (hasXDHAttribute(node, xdhWidget))
				digests += "(" + getOrGenerateId(node) + "|(" + getXDHAttribute(node, xdhWidget) + "))|";
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

// Deprecated
function getContent(elementOrId)	// Deprecated!
{
	var element = getElement(elementOrId);
	var tagName = element.tagName;
	var content = ""

	switch (tagName) {
		case "INPUT":
			switch (element.getAttribute("type")) {
				case "checkbox":
				case "radio":
					content =  element.checked;
					break;
				default:
					content =  element.value;
					break;
			}
			break;
		case "TEXTAREA":
			content =  element.value;
			break;
		case "SELECT":
			if (element.selectedIndex == -1)
				content =  "";
			else
				content =  element.options[element.selectedIndex].value;
			break;
		case "OPTION":
			content =  element.value;
			break;
		case "text":	// SVG
		case "tspan":	// SVG
			content =  element.textContent;
			break;
		default:
			content = hasXDHAttribute(element, xdhContent)
								? getXDHAttribute(element, xdhContent)
								: ( hasXDHAttribute(element, xdhOldContent)
									  ? getXDHAttribute(element, xdhOldContent)
										 : element.innerHTML );
			break;
	}

	return content;
}

function getValue(elementOrId)	// Returns the value of element of id 'id'.
{
	var element = getElement(elementOrId);
	var tagName = element.tagName;
	var value = ""

	switch (tagName) {
		case "INPUT":
			switch (element.getAttribute("type")) {
				case "checkbox":
				case "radio":
					if ( element.hasAttribute("value") )
						value = element.value;
					else
						value = element.checked;
					break;
				default:
					value =  element.value;
					break;
			}
			break;
		case "TEXTAREA":
			value =  element.value;
			break;
		case "SELECT":
			if (element.selectedIndex != -1)
				value = element.options[element.selectedIndex].value;
			break;
		case "OPTION":
			value =  element.value;
			break;
		case "text":	// SVG
		case "tspan":	// SVG
			value =  element.textContent;
			break;
		default:
			if ( hasXDHAttribute( element, xdhRadio ) ) {
				let checked = getCheckedRadio(element);
				if ( checked )
					value = checked.value;
			}	else
				value = element.innerHTML;
			break;
	}

	return value;
}

function getMark(idOrElement) {
	var element = getElement(idOrElement);
	mark = "";
	
	while ( (element !== null) && !hasXDHAttribute(element, xdhMark))
		element = element.parentNode;
	
	if ( element !== null )
		mark = getXDHAttribute(element, xdhMark);
	
	return mark;
}

function getWidgetRetrievingMethod( elementOrId ) {
    element = getElement(elementOrId);
    
    if (hasXDHAttribute(element, xdhWidgetContentRetrievingMethod))
        return getXDHAttribute(element, xdhWidgetContentRetrievingMethod);
    else
        return "";
    
}

function escapeQuotes(string) {
	return string.toString().replace(/\\/g,'\\\\').replace(/"/g,'\\"');
	// 'toString()' as 'string' could, for example, be a boolean.
}

function prependToFlatStrings(string, strings) {
	return '"' + escapeQuotes(string) + (strings ? '",' + strings : '"');
}

function appendToFlatStrings(string, strings) {
	return (strings ? strings + ',"' : '"' ) + escapeQuotes(string) + '"';
}

// Deprecated
function getContents(ids) {
	var i = ids.length;
	var contents = "";

	while(i--) {
        let element = getElement(ids[i]);
        let widgetRetrievingMethod = getWidgetRetrievingMethod(element);
        
        if ( widgetRetrievingMethod !== "" )
            contents = prependToFlatStrings(eval(widgetRetrievingMethod), contents);
        else
            contents = prependToFlatStrings(getContent(element), contents);
	}

	return contents;
}

function getValues(ids) {
	var i = ids.length;
	var values = "";

	while(i--) {
        let element = getElement(ids[i]);
        let widgetRetrievingMethod = getWidgetRetrievingMethod(element);
        
        if ( widgetRetrievingMethod !== "" )
            values = prependToFlatStrings(eval(widgetRetrievingMethod), values);
        else
            values = prependToFlatStrings(getValue(element), values);
	}

	return values;
}

function getMarks(ids) {
	var i = ids.length;
	var marks = "";

	while(i--)
		marks = prependToFlatStrings(getMark(getElement(ids[i])), marks);

	return marks;
}

function setEventHandlers(ids, events) {
	let i = ids.length;

	if (ids.length !== events.length)
		throw "Inconsistency !";

	while (i--) {
		getElement(ids[i]).addEventListener(events[i], handleEvent, false);
	}
}

function instantiateWidget(id, type, parameters, contentRetrievingMethod, focusingMethod, selectionMethod) {
    // 'focusingMethod' and 'selectionMethod' are currently not handled.
    
    element = getElement(id);
    
    if ( !hasXDHAttribute(element, xdhWidgetContentRetrievingMethod)) {
        setXDHAttribute(element, xdhWidgetContentRetrievingMethod, 'jQuery( getElement( "' + id + '") ).' + contentRetrievingMethod);
        
        return 'jQuery( getElement( "' + id + '") ).' + type + '(' + parameters + ');'
    } else
        return "";
}

function instantiateWidgets(ids, types, parametersSets, contentRetrievingMethods, focusingMethods, selectionMethods) {
	var i = ids.length;
	var script = "";

	if (ids.length !== types.length)
		throw "Inconsistency";

	if (ids.length !== parametersSets.length)
		throw "Inconsistency";

	while (i--) {
		script += instantiateWidget(ids[i], types[i], parametersSets[i], contentRetrievingMethods[i], focusingMethods[i], selectionMethods[i]);
	}

	eval(script);
}

function buildDigest(target, type, keys) {
	var digest = "";

	if (hasXDHAttribute(target, xdhOnEvent))
		digest += "(" + getXDHAttribute(target, xdhOnEvent) + ")";

	if (hasXDHAttribute(target, xdhOnEvents))
		digest += getXDHAttribute(target, xdhOnEvents);

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

	return getElement(id).sheet;
}

function insertCSSRule(id, rule, index) {
	var rules = getCSSRules(id);

	if (index === -1)
		index = rules.cssRules.length;

	rules.insertRule(rule, index);

	return index;
}

function removeCSSRule(id, index) {
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
	handleClasses(ids, classes, (id, clas) => getElement(id).classList.add(clas));
}

function removeClasses(ids, classes) {
	handleClasses(ids, classes, (id, clas) => getElement(id).classList.remove(clas));
}

function toggleClasses(ids, classes) {
	handleClasses(ids, classes, (id, clas) => getElement(id).classList.toggle(clas));
}

function enableElements(ids) {
	let i = 0;

	while (i < ids.length) {
		getElement(ids[i]).disabled = false;

		i++;
	}
}

function disableElements(ids) {
	let i = 0;

	while (i < ids.length) {
		getElement(ids[i]).disabled = true;

		i++;
	}
}
