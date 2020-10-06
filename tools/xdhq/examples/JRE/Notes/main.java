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

import info.q37.atlas.*;
import java.util.*;

class Note {
	String title = "", description = "";

	public Note(String title, String description) {
		this.title = title;
		this.description = description;
	}

	public Note() {
	}
}

class Notes extends Atlas {
	private String pattern = "";
	private boolean hideDescriptions = false;
	private int index = 0;
	private String viewModeElements[] = { "Pattern", "CreateButton", "DescriptionToggling", "ViewNotes" };
	private List<Note> notes;

	static private String readAsset_( String path )  {
		return readAsset( path, "Notes" );
	}

	private void push(Note note, int id, XML xml) {
		xml.pushTag( "Note" );
		xml.putAttribute( "id", id );
		xml.putTagAndValue("title", note.title);

		// Not used, due to d-o-e bug from Firefox (https://bugzilla.mozilla.org/show_bug.cgi?id=98168).
		xml.putTagAndValue( "description", note.description );

		xml.popTag();
	}

	private void handleDescriptions( DOM dom ) {
		if (hideDescriptions)
			dom.disableElement("ViewDescriptions");
		else
			dom.enableElement("ViewDescriptions");
	}

	private void displayList( DOM dom ) {
		XML xml = Atlas.createXML( "XDHTML" );
		Map<String,String> idsAndValues = new HashMap<String,String>();
		ListIterator<Note> li = notes.listIterator(1); // 0 skipped, as it serves as buffer for the new notes.

		xml.pushTag( "Notes" );

		while (li.hasNext()) {
			int index = li.nextIndex();
			Note note = li.next();
			int length = pattern.length();

			if ((length == 0) || pattern.equals(note.title.substring(0, length).toLowerCase())) {
				push(note, index, xml);
				idsAndValues.put(  "Description." + index, note.description );
			}
		}

		xml.popTag();

		dom.inner("Notes", xml, "Notes.xsl" );
		dom.setValues(idsAndValues);
		dom.enableElements(viewModeElements);
	}

	private void view( DOM dom ) {
		dom.enableElements(viewModeElements);
		dom.setValue("Edit." + index, "");
		index = -1;
	}

	private void edit(DOM dom, String id) {
		index = Integer.parseInt(id);
		Note note = notes.get(index);

		dom.inner("Edit." + id, readAsset_( "Note.html") );
		dom.setValues( new HashMap<String,String> () {{ put( "Title", note.title); put("Description", note.description); }} );
		dom.disableElements(viewModeElements);
		dom.focus("Title");
	}

	private void submit( DOM dom ) {
		String ids[] = { "Title", "Description" };
		String result[] = dom.getValues(ids);

		String title = result[0].trim();
		String description = result[1];

		if (!"".equals(title)) {
			notes.set(index, new Note(title, description));

			if (index == 0) {
				notes.add(0, new Note());
				displayList( dom );
			} else {
				dom.setValues( new HashMap<String,String> () {{ put( "Title." + index, title); put("Description." + index, description); }} );
				view( dom );
			}
		} else {
			dom.alert("Title can not be empty !");
			dom.focus( "Title");
		}
	}

	private void cancel( DOM dom ) {
		Note note = notes.get(index);
		String result[] = dom.getValues(new String[] { "Title", "Description" } );
		String title = result[0].trim();
		String description = result[1];

		if (!title.equals(note.title) || !description.equals(note.description)) {
			if (dom.confirm("Are you sure you want to cancel your modifications ?"))
				view( dom );
		} else
			view( dom );
	}

	public Notes() {
		// System.out.println("Connection detected...");
		notes = new ArrayList<Note>();

		// First must be empty as it used as buffer for the new notes.
		notes.add(new Note());
		notes.add(new Note("Improve design",
				"Tastes and colors... (aka «CSS aren't my cup of tea...»)"));
		notes.add(new Note("Fixing bugs", "There are bugs ? Really ?"));
		notes.add(new Note("Implement new functionalities", "Although it's almost perfect..., isn't it ?"));
	}

	@Override
	public void handle(String action, String id ) {
		if ( action.equals( "" ) ) {
			dom.inner("", readAsset_( "Main.html") );
			displayList( dom );
		} else if ( action.equals( "ToggleDescriptions" ) ) {
			hideDescriptions = "true".equals(dom.getValue(id));
			handleDescriptions( dom );
		} else if ( action.equals( "Search" ) ) {
			pattern = dom.getValue("Pattern").toLowerCase();
			displayList( dom );
		} else if ( action.equals( "Edit" ) ) {
			edit(dom, dom.getMark(id));
		} else if ( action.equals( "Delete" ) ) {
			if (dom.confirm("Are you sure you want to delete this entry ?")) {
				notes.remove(Integer.parseInt(dom.getMark(id)));
				displayList( dom );
			}
		} else if ( action.equals( "Submit" ) ) {
			submit( dom );
		} else if ( action.equals( "Cancel" ) ) {
			cancel( dom );
		} else {
			System.out.println("No or unknown action !");
			System.exit(1);
		}
	}
	public static void main(String args[]) {
		String dir;

		launch(() -> new Notes(),readAsset_("Head.html"), "Notes", GUI.DEFAULT, args);
	}
}
