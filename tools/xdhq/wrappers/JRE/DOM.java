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

import info.q37.xdhq.dom.SHRD.Type;

public class DOM {
	private info.q37.xdhq.dom.SHRD XDHq;
	private String[] empty = {};
	private String[][] emptys = {};

	private String[][] split(String[][] idsAndClasses) {
		List<String> ids = new ArrayList<String>();
		List<String> classes = new ArrayList<String>();

		for( String[] idAndClass : idsAndClasses ) {
			ids.add(idAndClass[0]);
			classes.add(idAndClass[1]);
		}

		return new String[][] { ids.toArray( new String[0] ), classes.toArray( new String[0] ) };
	}

	public DOM() {
		XDHq = new info.q37.xdhq.dom.PROD();
	}

	public String getAction( info.q37.xdhq.dom.Event event )
	{
		return XDHq.getAction( event );
	}

	public String execute(String script) {
		return (String)XDHq.call( "Execute_1", Type.STRING, new String[]{script}, emptys );
	}

	public void alert(String message) {
		XDHq.call("Alert_1", Type.VOID, new String[]{message}, emptys);
	}

	public boolean confirm(String message) {
		return (Boolean)XDHq.call("Confirm_1", Type.VOID, new String[]{message}, emptys );
	}

	void setLayout_(String id, String xml, String xslFilename) {
		XDHq.call("SetLayout_1", Type.VOID, new String[]{id, xml, xslFilename}, emptys );
	}

	public void setLayout(String id, String html) {
		setLayout_( id, html, "" );
	}

	public void setLayout(String id, String xml, String xslFilename) {
		setLayout_(id, xml, xslFilename);
	}

	public String[] getContents(String[] ids) {
		return (String [])XDHq.call("GetContents_1", Type.STRINGS, new String[0], new String[][]{ids});
	}

	public String getContent(String id) {
		return getContents( new String []{id} )[0];
	}

	public void setContents(String[][] idsAndContents) {
		String splittedIdsAndContents[][] = split(idsAndContents);

		XDHq.call("SetContents_1", Type.STRING, empty, splittedIdsAndContents);
	}

	public void setContent(String id, String content) {
		setContents( new String[][] { { id, content } } );
	}

	public void dressWidgets(String id) {
		XDHq.call("DressWidgets_1", Type.VOID, new String[]{id}, emptys );
	}

	private void handleClasses(String command, String[][] idsAndClasses) {
		String splittedIdsAndClasses[][] = split(idsAndClasses);

		XDHq.call(command, Type.VOID, empty, splittedIdsAndClasses);
	}

	private void handleClass(String command, String id, String clas) {
		handleClasses(command, new String[][] { { id, clas } } );
	}

	public void addClasses(String[][] idsAndClasses) {
		handleClasses("AddClasses_1", idsAndClasses);
	}

	public void addClass(String id, String clas) {
		handleClass("AddClasses_1", id, clas);
	}

	public void removeClasses(String[][] idsAndClasses) {
		handleClasses("RemoveClasses_1", idsAndClasses);
	}

	public void removeClass(String id, String clas) {
		handleClass("RemoveClasses_1", id, clas);
	}

	public void toggleClasses(String[][] idsAndClasses) {
		handleClasses("ToggleClasses_1", idsAndClasses);
	}

	public void toggleClass(String id, String clas) {
		handleClass("ToggleClasses_1", id, clas);
	}

	public void enableElements(String[] ids) {
		XDHq.call("EnableElements_1", Type.VOID, empty, new String[][] {ids});
	}

	public void enableElement(String id) {
		enableElements( new String[] { id } );
	}

	public void disableElements(String[] ids) {
		XDHq.call("DisableElements_1", Type.VOID, empty, new String[][] {ids});
	}

	public void disableElement(String id) {
		disableElements(new String[] { id } );
	}

	public void setAttribute(String id, String name, String value) {
		XDHq.call("SetAttribute_1", Type.VOID, new String[]{id, name, value}, emptys);
	}

	public String getAttribute(String id, String name) {
		return (String)XDHq.call("GetAttribute_1", Type.STRING, new String[] {id, name}, emptys);
	}

	public void removeAttribute(String id, String name) {
		XDHq.call("RemoveAttribute_1", Type.VOID, new String[]{id, name}, emptys);
	}

	public void setProperty(String id, String name, String value) {
		XDHq.call("SetProperty_1", Type.VOID, new String[] {id, name, value}, emptys);
	}

	public String getProperty(String id, String name) {
		return (String)XDHq.call("GetPoperty_1", Type.STRING, new String[] {id, name}, emptys );
	}

	public void focus(String id) {
		XDHq.call("Focus_1", Type.VOID, new String[]{id}, emptys );
	}
}
