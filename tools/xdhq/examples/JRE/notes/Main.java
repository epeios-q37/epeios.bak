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

	private void push(Note note, int index, Tree tree) {
		tree.pushTag("Note");
		tree.putAttribute("id", index);

		tree.pushTag("title");
		tree.putValue(note.title);
		tree.popTag();

		tree.pushTag("description");
		tree.putValue(note.description);
		tree.popTag();

		tree.popTag();
	}

	private void handleDescriptions( DOM dom ) {
		if (hideDescriptions)
			dom.disableElement("ViewDescriptions");
		else
			dom.enableElement("ViewDescriptions");
	}

	private void displayList( DOM dom ) {
		Tree tree = new Tree();
		List<String[]> idsAndContents = new ArrayList<String[]>();
		ListIterator<Note> li = notes.listIterator(1); // 0 skipped, as it serves as buffer for the new notes.

		tree.pushTag("Notes");

		while (li.hasNext()) {
			int index = li.nextIndex();
			Note note = li.next();
			int length = pattern.length();

			if ((length == 0) || pattern.equals(note.title.substring(0, length).toLowerCase())) {
				push(note, index, tree);
				idsAndContents.add( new String[] { "Description." + index, note.description } );
			}
		}

		dom.setLayout("Notes", tree, "Notes.xsl");
		dom.setContents(idsAndContents.toArray(new String[0][0]));
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

		dom.setLayout("Edit." + id, new Tree(), "Note.xsl");
		dom.setContents( new String[][] { { "Title", note.title }, { "Description", note.description } } );
		dom.disableElements(viewModeElements);
		dom.dressWidgets("Notes");
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
				dom.setContents( new String[][] { { "Title." + index, title }, { "Description." + index, description } } );
				view( dom );
			}
		} else
			dom.alert("Title can not be empty !");
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
				"Tastes and colors... (aka &laquo;CSS aren&rsquo;t my cup of tea...&raquo;)"));
		notes.add(new Note("Fixing bugs", "There are bugs ? Really ?"));
		notes.add(new Note("Implement new functionalities", "Although it&rsquo;s almost perfect..., isn&rsquo;t it ?"));
	}

	public void handle( DOM dom, String action, String id ) {
		if ( action.equals( "Connect" ) ) {
			dom.setLayout("", new Tree(), "Main.xsl");
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
		launch("Connect", "notes", GUI.DEFAULT, args );

		for (;;) new Notes();
	}
}