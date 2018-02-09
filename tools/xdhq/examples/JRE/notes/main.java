/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of UnJSq.

	UnJSq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	UnJSq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with UnJSq If not, see <http://www.gnu.org/licenses/>.
*/

import info.q37.unjsq.*;
import java.util.*;

class Note {
	String
		title = "",
		description = "";
	public Note( String title, String description ) {
		this.title = title;
		this.description = description;
	}
	public Note() {}
}

class Thread extends java.lang.Thread {
	private DOM dom;
	private String pattern = "";
	private boolean hideDescriptions = false;
	private int index = 0;
	private String viewModeElements[] = { "Pattern", "CreateButton", "DescriptionToggling", "ViewNotes" };
	private List<Note> notes;

	private void push( Note note, int index, Tree tree ) {
		tree.pushTag( "Note");
		tree.putAttribute( "id", index );

		tree.pushTag( "title" );
		tree.putValue( note.title );
		tree.popTag();

		tree.pushTag( "description" );
		tree.putValue( note.description );
		tree.popTag();

		tree.popTag();
	}

	private void handleDescriptions() {
		if ( hideDescriptions )
			dom.disableElement( "ViewDescriptions");
		else
			dom.enableElement( "ViewDescriptions");
	}

	private void displayList() {
		Tree tree = new Tree();
		List<String []> idsAndContents = new ArrayList<String []>();
		ListIterator<Note> li = notes.listIterator( 1 );	// 0 skipped, as it serves as buffer for the new notes.

		tree.pushTag( "Notes");
		tree.putAttribute( "HideDescriptions", "tutu");

		while ( li.hasNext() ) {
			int index = li.nextIndex();
			Note note = li.next();
			int length = pattern.length();

			if ( ( length == 0 ) || pattern.equals(note.title.substring(0, length).toLowerCase() ) ) {
				push( note, index, tree );
				String idAndContent[] = { "Description." + index, note.description };
				idsAndContents.add( idAndContent );
			}
		}

		dom.setLayout( "Notes", tree, "Notes.xsl");
		dom.setContents( idsAndContents.toArray( new String[0][0] ) );
		dom.enableElements( viewModeElements );
	}

	private void view() {
		dom.enableElements( viewModeElements );
		dom.setContent( "Edit." + index , "" );
		index = -1;
	}

	private void edit( String id ) {
		index = Integer.parseInt( id );
		Note note = notes.get( index );

		String idsAndContents[][] = {
			{ "Title", note.title },
			{ "Description", note.description }
		};

		dom.setLayout( "Edit." + id, new Tree(), "Note.xsl");
		dom.setContents( idsAndContents );
		dom.disableElements( viewModeElements );
		dom.dressWidgets( "Notes");
	}

	private void submit() {
		String ids[] = { "Title", "Description"};
		String result[] = dom.getContents( ids );

		String title = result[0].trim();
		String description = result[1];

		if ( !"".equals( title ) ) {
			notes.set(index,new Note( title, description ));

			if ( index == 0 ) {
				notes.add(0, new Note() );
				displayList();
			} else {
				String idsAndContents[][] = {
					{ "Title." + index, title },
					{ "Description." + index, description }
				};

				dom.setContents( idsAndContents );
				view();
			}
		} else
			dom.alert("Title can not be empty !");
	}

	private void cancel() {
		Note note = notes.get( index );
		String ids[] = { "Title", "Description" };
		String result[] = dom.getContents( ids );
		String title = result[0].trim();
		String description = result[1];

		if ( !title.equals(note.title) || !description.equals(note.description) ) {
			if ( dom.confirm("Are you sure you want to cancel your modifications ?" ) )
				view();
		} else
			view();
	}

	public Thread(DOM dom) {
		this.dom = dom;

		notes = new ArrayList<Note>();

		// First must be empty as it used as buffer for the new notes.
		notes.add( new Note() );
		notes.add( new Note( "Improve design", "Tastes and colors... (aka &laquo;CSS aren&rsquo;t my cup of tea...&raquo;)" ) );
		notes.add( new Note( "Fixing bugs", "There are bugs ? Really ?" ) );
		notes.add( new Note( "Implement new functionalities", "Although it&rsquo;s almost perfect..., isn&rsquo;t it ?" ) );
	}

	public void run() {
		Event event = new Event();

		System.out.println("Connection detected...");

		for (;;) {
			switch (dom.getAction(event)) {
			case "Connect":
				dom.setLayout("", new Tree(), "Main.xsl");
				displayList();
				break;
			case "ToggleDescriptions":
				hideDescriptions = "true".equals( dom.getContent( event.id ));
				handleDescriptions();
				break;
			case "Search":
				pattern = dom.getContent( "Pattern").toLowerCase();
				displayList();
				break;
			case "Edit":
				edit( dom.getContent( event.id ));
				break;
			case "Delete":
				if ( dom.confirm("Are you sure you want to delete this entry ?" )) {
					notes.remove(Integer.parseInt(dom.getContent(event.id)));
					displayList();
				}
				break;
			case "Submit":
				submit();
				break;
			case "Cancel":
				cancel();
				break;
			default:
				System.out.println( "No or unknown action !");
				System.exit( 1 );
				break;
			}
		}
	}
}

class Notes {
	public static void main(String[] args) throws Exception {
		UnJSq.listen( "notes" );

		for (;;) {
			java.lang.Thread thread = new Thread(new DOM());
			thread.start();
		}
	}
}