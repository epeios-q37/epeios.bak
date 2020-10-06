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

	public void flush() {
		DOM.call("Flush_1", Type.STRING);
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

	public <XML> void prependLayout(String id, XML html) {	// Deprecated!
		handleLayout_( "Prepend", id, html, "");
	}

	public <XML> void setLayout(String id, XML html) {	// Deprecated!
		handleLayout_( "Set",id, html, "");
	}

	public <XML> void appendLayout(String id, XML html) {	// Deprecated!
		handleLayout_( "Append",id, html, "");
	}
	   	 
	private <XML> void handleLayoutXSL_(String variant, String id, XML xml, String xslFilename) {	// Deprecated!
		String xslURL = xslFilename;

		if ( info.q37.xdhq.XDH.isFaaS() )
			xslURL = new String( "data:text/xml;base64," + java.util.Base64.getEncoder().encodeToString( info.q37.xdhq.XDH.readAsset( xslFilename, info.q37.xdhq.XDH.getDir() ).getBytes() ) );
		
		handleLayout_(variant, id, xml, xslURL);
	}

	public <XML> void prependLayoutXSL(String id, XML xml, String xslFilename) {	// Deprecated!
		handleLayoutXSL_( "Prepend",id, xml, xslFilename);
	}

	public <XML> void setLayoutXSL(String id, XML xml, String xslFilename) {	// Deprecated!
		handleLayoutXSL_( "Set",id, xml, xslFilename);
	}

	public <XML> void appendLayoutXSL(String id, XML xml, String xslFilename) {	// Deprecated!
		handleLayoutXSL_( "Append",id, xml, xslFilename);
	}

	private <XML> void layout_(String variant, String id, XML xml, String xsl) {
		if (!xsl.isEmpty() && info.q37.xdhq.XDH.isFaaS())
			xsl = new String( "data:text/xml;base64," + java.util.Base64.getEncoder().encodeToString( info.q37.xdhq.XDH.readAsset( xsl, info.q37.xdhq.XDH.getDir() ).getBytes() ) );

		DOM.call("HandleLayout_1", Type.VOID, a(variant), a(id), a(xml.toString()), a(xsl));
	}

	public <XML> void before(String id, XML xml, String xsl) {
		layout_("beforebegin", id, xml, xsl);
	}
	   	 
	public <XML> void before(String id, XML xml) {
		before(id, xml, "");
	}

	public <XML> void begin(String id, XML xml, String xsl) {
		layout_("afterbegin", id, xml, xsl);
	}
	   	 
	public <XML> void begin(String id, XML xml) {
		begin(id, xml, "");
	}

	public <XML> void inner(String id, XML xml, String xsl) {
		layout_("inner", id, xml, xsl);
	}
	   	 
	public <XML> void inner(String id, XML xml) {
		inner(id, xml, "");
	}

	public <XML> void end(String id, XML xml, String xsl) {
		layout_("beforeend", id, xml, xsl);
	}
	   	 
	public <XML> void end(String id, XML xml) {
		end(id, xml, "");
	}

	public <XML> void after(String id, XML xml, String xsl) {
		layout_("afterend", id, xml, xsl);
	}
	   	 
	public <XML> void after(String id, XML xml) {
		after(id, xml, "");
	}
	
	// Deprecated
	public String[] getContents(String[] ids) {
		return (String[]) DOM.call("GetContents_1", Type.STRINGS, a(ids));
	}

	// Deprecated
	public String getContent(String id) {
		return getContents(new String[] { id })[0];
	}

	// Deprecated
	public void setContents(Map<String, String> idsAndContents) {
		String[][] splittedIdsAndContents = split(idsAndContents);
		DOM.call("SetContents_1", Type.VOID, a(splittedIdsAndContents[0]), a(splittedIdsAndContents[1]));
	}

	// Deprecated
	public void setContent(final String id, final String content) {
		setContents(new HashMap<String, String>() {
			{
				put(id, content);
			}
		});
	}

	public String[] getValues(String[] ids) {
		return (String[]) DOM.call("GetValues_1", Type.STRINGS, a(ids));
	}

	public String getValue(String id) {
		return getValues(new String[] { id })[0];
	}

	public void setValues(Map<String, String> idsAndValues) {
		String[][] splittedIdsAndValues = split(idsAndValues);
		DOM.call("SetValues_1", Type.VOID, a(splittedIdsAndValues[0]), a(splittedIdsAndValues[1]));
	}

	public void setValue(final String id, final String value) {
		setValues(new HashMap<String, String>() {
			{
				put(id, value);
			}
		});
	}

	public String[] getMarks(String[] ids) {
		return (String[]) DOM.call("GetMarks_1", Type.STRINGS, a(ids));
	}

	public String getMark(String id) {
		return getMarks(new String[] { id })[0];
	}

	public void setMarks(Map<String, String> idsAndMarks) {
		String[][] splittedIdsAndMarks = split(idsAndMarks);
		DOM.call("SetMarks_1", Type.VOID, a(splittedIdsAndMarks[0]), a(splittedIdsAndMarks[1]));
	}

	public void setMark(final String id, final String mark) {
		setMarks(new HashMap<String, String>() {
			{
				put(id, mark);
			}
		});
	}

/*
	public String createElement( String name, String id ) {
		return (String)DOM.call( "CreateElement_1", Type.STRING, new String[]{name, id }, emptys );
	}

	public String createElement(String name) {
		return createElement(name, "");
	}

	public void insertChild( String child, String id ) {
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

	public String parent(String id) {
		return (String) DOM.call("Parent_1", Type.STRING, a(id));
	}

	public String firstChild(String id) {
		return (String) DOM.call("FirstChild_1", Type.STRING, a(id));
	}

	public String lastChild(String id) {
		return (String) DOM.call("LastChild_1", Type.STRING, a(id));
	}

	public String previousSibling(String id) {
		return (String) DOM.call("PreviousSibling_1", Type.STRING, a(id));
	}

	public String nextSibling(String id) {
		return (String) DOM.call("NextSibling_1", Type.STRING, a(id));
	}

	public void scrollTo(String id) {
		DOM.call("ScrollTo_1", Type.VOID, a(id));
	}
}
