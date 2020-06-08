/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

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

package info.q37.xdhq;

import java.util.*;

import info.q37.xdhq.dom.DOM_SHRD.Type;
import info.q37.xdhq.MODE;
import info.q37.xdhq.ARG;

public class DOM {
	private info.q37.xdhq.dom.DOM_SHRD DOM;

	private String[][] split(Map<String, String> idsAndClasses) {
		return new String[][] { idsAndClasses.keySet().toArray(new String[0]),
				idsAndClasses.values().toArray(new String[0]) };
	}

	public DOM(MODE mode) {
		try {
			switch (mode) {
			case FAAS:
				DOM = new info.q37.xdhq.dom.DOM_FAAS();
				break;
			case SLFH:
				DOM = new info.q37.xdhq.dom.DOM_SLFH();
				break;
			default:
				throw new RuntimeException("Unknown mode !!!");
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public info.q37.xdhq.dom.DOM_SHRD getDOM() {
		return DOM;
	}

	public boolean isQuitting() {
		return DOM.isQuitting();
	}

	public void getAction(info.q37.xdhq.dom.Event event) {
		DOM.getAction(event);
	}

	static private ARG a( String string ) {
		return new ARG(string );
	}

	static private ARG a( String[] strings ) {
		return new ARG(strings);
	}

	public void executeVoid(String script) {
		DOM.call("Execute_1", Type.VOID, a(script));
	}

	public String executeString(String script) {
		return (String) DOM.call("Execute_1", Type.STRING, a(script));
	}

	public String[] executeStrings(String script) {
		return (String []) DOM.call("Execute_1", Type.STRINGS, a(script));
	}

	public void alert(String message) {
		DOM.call("Alert_1", Type.STRING, a(message));
		// For the return value being 'STRING' instead of 'VOID',
		// see the 'alert' primitive in 'XDHqXDH'.
	}

	public boolean confirm(String message) {
		return "true".equals(DOM.call("Confirm_1", Type.STRING, a(message)));
	}

	private <XML> void handleLayout_(String variant, String id, XML xml, String xslFilename) {
		DOM.call("HandleLayout_1", Type.VOID, a(variant), a(id), a(xml.toString()), a(xslFilename));
	}

	public <XML> void prependLayout(String id, XML html) {
		handleLayout_( "Prepend", id, html, "");
	}

	public <XML> void setLayout(String id, XML html) {
		handleLayout_( "Set",id, html, "");
	}

	public <XML> void appendLayout(String id, XML html) {
		handleLayout_( "Append",id, html, "");
	}
	   	 
	private <XML> void handleLayoutXSL_(String variant, String id, XML xml, String xslFilename) {
		String xslURL = xslFilename;

		if ( info.q37.xdhq.XDH.isFaaS() )
			xslURL = new String( "data:text/xml;base64," + java.util.Base64.getEncoder().encodeToString( info.q37.xdhq.XDH.readAsset( xslFilename, info.q37.xdhq.XDH.getDir() ).getBytes() ) );
		
		handleLayout_(variant, id, xml, xslURL);
	}

	public <XML> void prependLayoutXSL(String id, XML xml, String xslFilename) {
		handleLayoutXSL_( "Prepend",id, xml, xslFilename);
	}

	public <XML> void setLayoutXSL(String id, XML xml, String xslFilename) {
		handleLayoutXSL_( "Set",id, xml, xslFilename);
	}

	public <XML> void appendLayoutXSL(String id, XML xml, String xslFilename) {
		handleLayoutXSL_( "Append",id, xml, xslFilename);
	}
	   	 
	public String[] getContents(String[] ids) {
		return (String[]) DOM.call("GetContents_1", Type.STRINGS, a(ids));
	}

	public String getContent(String id) {
		return getContents(new String[] { id })[0];
	}

	public final void setContents(Map<String, String> idsAndContents) {
		String[][] splittedIdsAndContents = split(idsAndContents);
		DOM.call("SetContents_1", Type.VOID, a(splittedIdsAndContents[0]), a(splittedIdsAndContents[1]));
	}

	public final void setContent(final String id, final String content) {
		setContents(new HashMap<String, String>() {
			{
				put(id, content);
			}
		});
	}

/*
	public final String createElement( String name, String id ) {
		return (String)DOM.call( "CreateElement_1", Type.STRING, new String[]{name, id }, emptys );
	}

	public final String createElement(String name) {
		return createElement(name, "");
	}

	public final void insertChild( String child, String id ) {
		DOM.call( "InsertChild_1", Type.VOID, new String[]{ child, id }, emptys );
	}
*/

	private void handleClasses(String variant, Map<String, String> idsAndClasses) {
		String splittedIdsAndClasses[][] = split(idsAndClasses);

		DOM.call("HandleClasses_1", Type.VOID, a(variant), a(splittedIdsAndClasses[0]),a(splittedIdsAndClasses[1]));
	}

	private void handleClass(String variant, final String id, final String clas) {
		handleClasses(variant, new HashMap<String, String>() {
			{
				put(id, clas);
			}
		});
	}

	public void addClasses(Map<String, String> idsAndClasses) {
		handleClasses("Add", idsAndClasses);
	}

	public void addClass(String id, String clas) {
		handleClass("Add", id, clas);
	}

	public void removeClasses(Map<String, String> idsAndClasses) {
		handleClasses("Remove", idsAndClasses);
	}

	public void removeClass(String id, String clas) {
		handleClass("Remove", id, clas);
	}

	public void toggleClasses(Map<String, String> idsAndClasses) {
		handleClasses("Toggle", idsAndClasses);
	}

	public void toggleClass(String id, String clas) {
		handleClass("Toggle", id, clas);
	}

	public void enableElements(String[] ids) {
		DOM.call("EnableElements_1", Type.VOID, a(ids));
	}

	public void enableElement(String id) {
		enableElements(new String[] { id });
	}

	public void disableElements(String[] ids) {
		DOM.call("DisableElements_1", Type.VOID, a(ids));
	}

	public void disableElement(String id) {
		disableElements(new String[] { id });
	}

	public void setAttribute(String id, String name, String value) {
		DOM.call("SetAttribute_1", Type.VOID, a(id), a(name), a(value));
	}

	public String getAttribute(String id, String name) {
		return (String) DOM.call("GetAttribute_1", Type.STRING, a(id), a(name));
	}

	public void removeAttribute(String id, String name) {
		DOM.call("RemoveAttribute_1", Type.VOID, a(id), a(name));
	}

	public void setProperty(String id, String name, String value) {
		DOM.call("SetProperty_1", Type.VOID, a(id), a(name), a(value));
	}

	public String getProperty(String id, String name) {
		return (String) DOM.call("GetPoperty_1", Type.STRING, a(id), a(name));
	}

	public void focus(String id) {
		DOM.call("Focus_1", Type.VOID, a(id));
	}
}
