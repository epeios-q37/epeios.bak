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

public class DOM {
	private Object core;

	private String[][] split(String[][] idsAndClasses) {
		List<String> ids = new ArrayList<String>();
		List<String> classes = new ArrayList<String>();

		for( String[] idAndClass : idsAndClasses ) {
			ids.add(idAndClass[0]);
			classes.add(idAndClass[1]);
		}

		String [][] ret = { ids.toArray(new String[0]), classes.toArray(new String[0]) };

		return ret;
	}

	public DOM() {
		core = XDHq.call(8);
	}

	public void finalize() {
		XDHq.call(9, core);
	}

	// Not very elegant !
	/*
	private Object call(int index, Object... objects) {
		// With below line, 'objects' is one argument, and is not exploded.
		 // return XDHq.call(index, core, objects);

		switch (objects.length) {
		case 1:
			return XDHq.call(index, core, objects[0]);
		case 2:
			return XDHq.call(index, core, objects[0], objects[1]);
		case 3:
			return XDHq.call(index, core, objects[0], objects[1], objects[2]);
		default:
			System.err.println("Missing case !");
			System.exit(1);
			return null;
		}
	}
	*/

	public void set(Object object) {
		XDHq.call(9, core, object);
	}

	public String getAction(Event event) {
		XDHq.call(10, core, event);

		return event.action;
	}

	public String execute(String script) {
		return (String)XDHq.call(11, core, script);
	}

	public void alert(String message) {
		XDHq.call(12, core, message);
	}

	public boolean confirm(String message) {
		return (Boolean)XDHq.call(13, core, message);
	}

	public void setLayout(String id, Tree tree, String xslFilename) {
		XDHq.call(14, core, id, tree.core(), xslFilename);
	}

	public String[] getContents(String[] ids) {
		return (String [])XDHq.call(15, core, ids);
	}

	public String getContent(String id) {
		String ids[] = { id };

		return getContents(ids)[0];
	}

	public void setContents(String[][] idsAndContents) {
		String splittedIdsAndContents[][] = split(idsAndContents);

		XDHq.call(16, core, splittedIdsAndContents[0], splittedIdsAndContents[1]);
	}

	public void setContent(String id, String content) {
		String idsAndContents[][] = { { id, content } };

		setContents(idsAndContents);
	}

	public void dressWidgets(String id) {
		XDHq.call(17, core, id);
	}

	private void handleClasses(int index, String[][] idsAndClasses) {
		String splittedIdsAndClasses[][] = split(idsAndClasses);

		XDHq.call(index, core, splittedIdsAndClasses[0], splittedIdsAndClasses[1]);
	}

	private void handleClass(int Index, String id, String clas) {
		String idAndClass[][] = { { id, clas } };
		handleClasses(Index, idAndClass);
	}

	public void addClasses(String[][] idsAndClasses) {
		handleClasses(18, idsAndClasses);
	}

	public void addClass(String id, String clas) {
		handleClass(18, id, clas);
	}

	public void removeClasses(String[][] idsAndClasses) {
		handleClasses(19, idsAndClasses);
	}

	public void removeClass(String id, String clas) {
		handleClass(19, id, clas);
	}

	public void toggleClasses(String[][] idsAndClasses) {
		handleClasses(20, idsAndClasses);
	}

	public void toggleClass(String id, String clas) {
		handleClass(20, id, clas);
	}

	public void enableElements(String[] ids) {
		XDHq.call(21, core, ids);
	}

	public void enableElement(String id) {
		String ids[] = { id };

		enableElements(ids);
	}

	public void disableElements(String[] ids) {
		XDHq.call(22, core, ids);
	}

	public void disableElement(String id) {
		String ids[] = { id };

		disableElements(ids);
	}

	public void setAttribute(String id, String name, String value) {
		XDHq.call(23, core, id, name, value);
	}

	public String getAttribute(String id, String name) {
		return (String)XDHq.call(24, core, id, name);
	}

	public void removeAttribute(String id, String name) {
		XDHq.call(25, core, id, name);
	}

	public void setProperty(String id, String name, String value) {
		XDHq.call(26, core, id, name, value);
	}

	public String getProperty(String id, String name) {
		return (String)XDHq.call(27, core, id, name);
	}

	public void focus(String id) {
		XDHq.call(28, core, id );
	}
}
