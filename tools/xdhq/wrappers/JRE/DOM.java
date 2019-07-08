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

public class DOM {
	private info.q37.xdhq.dom.DOM_SHRD DOM;
	private static String[] empty = {};
	private static String[][] emptys = {};

	private String[][] split(Map<String, String> idsAndClasses) {
		return new String[][] { idsAndClasses.keySet().toArray(new String[0]),
				idsAndClasses.values().toArray(new String[0]) };
	}

	public DOM(MODE mode) {
		try {
			switch (mode) {
			case DEMO:
				DOM = new info.q37.xdhq.dom.DOM_DEMO();
				break;
			case PROD:
				DOM = new info.q37.xdhq.dom.DOM_PROD();
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

	public void getAction(info.q37.xdhq.dom.Event event) {
		DOM.getAction(event);
	}

	public String execute(String script) {
		return (String) DOM.call("Execute_1", Type.STRING, new String[] { script }, emptys);
	}

	public void alert(String message) {
		DOM.call("Alert_1", Type.STRING, new String[] { message }, emptys);
		// For the return value being 'STRING' instead of 'VOID',
		// see the 'alert' primitive in 'XDHqXDH'.
	}

	public boolean confirm(String message) {
		return "true".equals(DOM.call("Confirm_1", Type.STRING, new String[] { message }, emptys));
	}

	private <XML> void handleLayout_(String command, String id, XML xml, String xslFilename) {
		DOM.call(command, Type.VOID, new String[] { id, xml.toString(), xslFilename }, emptys);
	}

	public <XML> void prependLayout(String id, XML html) {
		handleLayout_( "PrependLayout_1", id, html, "");
	}

	public <XML> void setLayout(String id, XML html) {
		handleLayout_( "SetLayout_1",id, html, "");
	}

	public <XML> void appendLayout(String id, XML html) {
		handleLayout_( "AppendLayout_1",id, html, "");
	}
	   	 
	private <XML> void handleLayoutXSL_(String command, String id, XML xml, String xslFilename) {
		String xslURL = xslFilename;

		if ( info.q37.xdhq.XDH.isDEMO() )
			xslURL = new String( "data:text/xml;base64," + java.util.Base64.getEncoder().encodeToString( info.q37.xdhq.XDH.readAsset( xslFilename, info.q37.xdhq.XDH.getDir() ).getBytes() ) );
		
		handleLayout_(command, id, xml, xslURL);
	}

	public <XML> void prependLayoutXSL(String id, XML xml, String xslFilename) {
		handleLayoutXSL_( "PrependLayout_1",id, xml, xslFilename);
	}

	public <XML> void setLayoutXSL(String id, XML xml, String xslFilename) {
		handleLayoutXSL_( "SetLayout_1",id, xml, xslFilename);
	}

	public <XML> void appendLayoutXSL(String id, XML xml, String xslFilename) {
		handleLayoutXSL_( "AppendLayout_1",id, xml, xslFilename);
	}
	   	 
	public String[] getContents(String[] ids) {
		return (String[]) DOM.call("GetContents_1", Type.STRINGS, empty, new String[][] { ids });
	}

	public String getContent(String id) {
		return getContents(new String[] { id })[0];
	}

	public final void setContents(Map<String, String> idsAndContents) {
		DOM.call("SetContents_1", Type.VOID, empty, split(idsAndContents));
	}

	public final void setContent(final String id, final String content) {
		setContents(new HashMap<String, String>() {
			{
				put(id, content);
			}
		});
	}

	public final void setTimeout(int delay, String action) {
		DOM.call("SetTimeout_1", Type.VOID, new String[] { Integer.toString(delay), action }, emptys);
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
	public void dressWidgets(String id) {
		DOM.call("DressWidgets_1", Type.VOID, new String[] { id }, emptys);
	}

	private void handleClasses(String command, Map<String, String> idsAndClasses) {
		String splittedIdsAndClasses[][] = split(idsAndClasses);

		DOM.call(command, Type.VOID, empty, splittedIdsAndClasses);
	}

	private void handleClass(String command, final String id, final String clas) {
		handleClasses(command, new HashMap<String, String>() {
			{
				put(id, clas);
			}
		});
	}

	public void addClasses(Map<String, String> idsAndClasses) {
		handleClasses("AddClasses_1", idsAndClasses);
	}

	public void addClass(String id, String clas) {
		handleClass("AddClasses_1", id, clas);
	}

	public void removeClasses(Map<String, String> idsAndClasses) {
		handleClasses("RemoveClasses_1", idsAndClasses);
	}

	public void removeClass(String id, String clas) {
		handleClass("RemoveClasses_1", id, clas);
	}

	public void toggleClasses(Map<String, String> idsAndClasses) {
		handleClasses("ToggleClasses_1", idsAndClasses);
	}

	public void toggleClass(String id, String clas) {
		handleClass("ToggleClasses_1", id, clas);
	}

	public void enableElements(String[] ids) {
		DOM.call("EnableElements_1", Type.VOID, empty, new String[][] { ids });
	}

	public void enableElement(String id) {
		enableElements(new String[] { id });
	}

	public void disableElements(String[] ids) {
		DOM.call("DisableElements_1", Type.VOID, empty, new String[][] { ids });
	}

	public void disableElement(String id) {
		disableElements(new String[] { id });
	}

	public void setAttribute(String id, String name, String value) {
		DOM.call("SetAttribute_1", Type.VOID, new String[] { id, name, value }, emptys);
	}

	public String getAttribute(String id, String name) {
		return (String) DOM.call("GetAttribute_1", Type.STRING, new String[] { id, name }, emptys);
	}

	public void removeAttribute(String id, String name) {
		DOM.call("RemoveAttribute_1", Type.VOID, new String[] { id, name }, emptys);
	}

	public void setProperty(String id, String name, String value) {
		DOM.call("SetProperty_1", Type.VOID, new String[] { id, name, value }, emptys);
	}

	public String getProperty(String id, String name) {
		return (String) DOM.call("GetPoperty_1", Type.STRING, new String[] { id, name }, emptys);
	}

	public void focus(String id) {
		DOM.call("Focus_1", Type.VOID, new String[] { id }, emptys);
	}
}
