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
	along with XDHq If not, see <http://www.gnu.org/licenses/>.
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

	private static String readAsset_( String path ) {
		String dir;

		if (System.getenv("EPEIOS_SRC") == null)
			dir = ".";
		else
			dir = "notes";

		return readAsset( path, dir );
	}

	private void push(Note note, int index, XML xml) {
		xml.pushTag( "Note" );
		xml.setAttribute( "id", + index );
		xml.pushTag("title");
		xml.setValue( note.title );
		xml.popTag();

		// Not used, due to d-o-e bug from Firefox (https://bugzilla.mozilla.org/show_bug.cgi?id=98168).
		xml.pushTag( "description");
		xml.setValue( note.description );
		xml.popTag();

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
		Map<String,String> idsAndContents = new HashMap<String,String>();
		ListIterator<Note> li = notes.listIterator(1); // 0 skipped, as it serves as buffer for the new notes.

		xml.pushTag( "Notes" );

		while (li.hasNext()) {
			int index = li.nextIndex();
			Note note = li.next();
			int length = pattern.length();

			if ((length == 0) || pattern.equals(note.title.substring(0, length).toLowerCase())) {
				push(note, index, xml);
				idsAndContents.put(  "Description." + index, note.description );
			}
		}

		xml.popTag();

		dom.setLayoutXSL("Notes", xml, "Notes.xsl" );
		dom.setContents(idsAndContents);
		dom.enableElements(viewModeElements);
	}

	private void view( DOM dom ) {
		dom.enableElements(viewModeElements);
		dom.setContent("Edit." + index, "");
		index = -1;
	}

	private void edit(DOM dom, String id) {
		index = Integer.parseInt(id);
		Note note = notes.get(index);

		dom.setLayout("Edit." + id, readAsset_( "Note.html") );
		dom.setContents( new HashMap<String,String> () {{ put( "Title", note.title); put("Description", note.description); }} );
		dom.disableElements(viewModeElements);
		dom.dressWidgets("Notes");
		dom.focus("Title");
	}

	private void submit( DOM dom ) {
		String ids[] = { "Title", "Description" };
		String result[] = dom.getContents(ids);

		String title = result[0].trim();
		String description = result[1];

		if (!"".equals(title)) {
			notes.set(index, new Note(title, description));

			if (index == 0) {
				notes.add(0, new Note());
				displayList( dom );
			} else {
				dom.setContents( new HashMap<String,String> () {{ put( "Title." + index, title); put("Description." + index, description); }} );
				view( dom );
			}
		} else {
			dom.alert("Title can not be empty !");
			dom.focus( "Title");
		}
	}

	private void cancel( DOM dom ) {
		Note note = notes.get(index);
		String result[] = dom.getContents(new String[] { "Title", "Description" } );
		String title = result[0].trim();
		String description = result[1];

		if (!title.equals(note.title) || !description.equals(note.description)) {
			if (dom.confirm("Are you sure you want to cancel your modifications ?"))
				view( dom );
		} else
			view( dom );
	}

	public Notes() {
		System.out.println("Connection detected...");
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
			dom.setLayout("", readAsset_( "Main.html") );
			displayList( dom );
		} else if ( action.equals( "ToggleDescriptions" ) ) {
			hideDescriptions = "true".equals(dom.getContent(id));
			handleDescriptions( dom );
		} else if ( action.equals( "Search" ) ) {
			pattern = dom.getContent("Pattern").toLowerCase();
			displayList( dom );
		} else if ( action.equals( "Edit" ) ) {
			edit(dom, dom.getContent(id));
		} else if ( action.equals( "Delete" ) ) {
			if (dom.confirm("Are you sure you want to delete this entry ?")) {
				notes.remove(Integer.parseInt(dom.getContent(id)));
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
	public static void main(String args[]) throws Exception {
		String dir;

		if (System.getenv("EPEIOS_SRC") == null)
			dir = ".";
		else
			dir = "notes";

		launch(() -> new Notes(),readAsset_("Head.html"), dir, GUI.DEFAULT, args);
	}
}