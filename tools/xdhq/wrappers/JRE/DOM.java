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

import info.q37.xdhq.*;
import java.util.*;

public class DOM {
	private Object core;

	private String[][] split( String[][] idsAndClasses ) {
		List<String> ids = new ArrayList<String>();
		List<String> classes = new ArrayList<String>();

		for( String[] idAndClass : idsAndClasses ) {
			ids.add( idAndClass[0] );
			classes.add( idAndClass[1] );
		}

		String [][] ret = { ids.toArray( new String[0] ), classes.toArray(new String[0]) };

		return ret;
	}

	public DOM() {
		System.out.println( "Av.!");
		core = XDHq.call(8);
		System.out.println( "Ap.!");
	}

	public void finalize() {
		XDHq.call(9, core);
	}

	private Object call( int index, Object... objects) {
		// With below line, 'objects' is one argument, and is not exploded.
		 // return XDHq.call( index, core, objects );

		switch ( objects.length ) {
		case 1:
			return XDHq.call( index, core, objects[0] );
		case 2:
			return XDHq.call( index, core, objects[0], objects[1] );
		case 3:
			return XDHq.call( index, core, objects[0], objects[1], objects[2] );
		default:
			System.err.println( "Missing case !");
			System.exit( 1 );
			return null;
		}

	}

	public void set(Object object) {
		call(10, object);
	}

	public void getAction(info.q37.xdhq.XDHqEvent data) {
		call(11, data);
	}

	public void setLayout(String id, Tree tree, String xslFilename) {
		call(14, id, tree.core(), xslFilename);
	}

	public String[] getContents( String[] ids ) {
		return (String [])call( 15, (Object [])ids );
	}

	public String getContent( String id ) {
		String ids[] = { id };

		return getContents( ids )[0];
	}

	public void setContents( String[][] idsAndContents ) {
		String splittedIdsAndContents[][] = split( idsAndContents );

		call( 16, splittedIdsAndContents[0], splittedIdsAndContents[1] );
	}

	public void setContent( String id, String content ) {
		String idsAndContents[][] = { { id, content } };

		setContents( idsAndContents );
	}

	private void handleClasses( int index, String[][] idsAndClasses) {
		String splittedIdsAndClasses[][] = split( idsAndClasses );

		call( index, splittedIdsAndClasses[0], splittedIdsAndClasses[1] );
	}

	private void handleClass( int Index, String id, String clas ) {
		String idAndClass[][] = { { id, clas } };
		handleClasses( Index, idAndClass );
	}

	public void addClasses( String[][] idsAndClasses ) {
		handleClasses( 18, idsAndClasses );
	}

	public void addClass( String id, String clas ) {
		handleClass( 18, id, clas );
	}

	public void removeClasses( String[][] idsAndClasses ) {
		handleClasses( 19, idsAndClasses );
	}

	public void removeClass( String id, String clas ) {
		handleClass( 19, id, clas );
	}

	public void toggleClasses( String[][] idsAndClasses ) {
		handleClasses( 20, idsAndClasses );
	}

	public void toggleClass( String id, String clas ) {
		handleClass( 20, id, clas );
	}
}
