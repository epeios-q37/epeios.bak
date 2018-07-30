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

package info.q37.xdhq.dom;

import java.net.*;
import java.io.*;

public class DOM_DEMO extends DOM_SHRD {
	private String address = "atlastk.org";
	private int port = 53800;
	private String token = "";
	private Socket socket;

	private void writeSize_( int size, OutputStream stream ) throws Exception {
		byte datum = (byte)(size & 0x7F);
		size >>= 7;

		while ( size != 0 ) {
			stream.write( datum |= 0x80 );
			datum = (byte)(size & 0x7F);
			size >>= 7;
		}

		stream.write( datum );
	}

	private void writeString_( String string, OutputStream stream ) throws Exception {
		byte bytes[] = string.getBytes();

		writeSize_( bytes.length, stream );
		stream.write( bytes );
	}

	private void writeString_( String string ) throws Exception {
		writeString_( string, socket.getOutputStream() );
	}

	private void writeStrings_( String[] strings, OutputStream stream ) throws Exception {
		int size = strings.length;
		int i = 0;

		writeSize_( size, stream );

		while ( i < size ) {
			writeString_( strings[i++], stream );
		}
	}

	private void writeStrings_( String[] strings ) throws Exception {
		writeStrings_( strings, socket.getOutputStream() );
	}



	private int getSize_( InputStreamReader reader ) throws Exception {
		int datum = reader.read();
		int size = datum & 0x7f;

		while ( (datum & 0x80) != 0 ) {
			datum = reader.read();

			size = (size << 7) + (datum & 0x7f);
		}

		return size;
	}

	private String getString_( InputStreamReader reader ) throws Exception {
		String string = "";
		int size = getSize_( reader );

		while ( size-- != 0 )
			string += reader.read();

		return string;
	}

	private String getQuery_( InputStreamReader reader ) throws Exception {
		String query = "";
		int datum = reader.read();

		while ( datum != 0 ) {
			query += datum;
			datum = reader.read();
		}

		return query;
	}

	private String getQuery_() throws Exception {
		return getQuery_( new InputStreamReader( socket.getInputStream() ) );
	}

	private String getString_() throws Exception {
		return getString_( new InputStreamReader( socket.getInputStream() ) );
	}

	public DOM_DEMO() throws Exception {
		socket = new Socket( address, port );
		OutputStream output = socket.getOutputStream();

		output.write(token.getBytes());

		token = getString_();

		if ( "".equals( token ) )
			throw new Exception( "Invalid connection information !!!");

		output.write(new String( "StandBy_1" + 0x00).getBytes() );
	}

	@Override public void getAction(Event event) {
		try {
			String query = getQuery_();

			if ( !"Launch_1".equals( query ) )
				throw new Exception( "Unknown query '" + query + "' !!!" );

			event.id = getString_();
			event.action = getString_();
		} catch ( Exception e) {
			e.printStackTrace();
		}
	}

	@Override public Object call(String command, Type type, String[] strings, String[][] xstrings )  {
		Object object = null;

		try {
			OutputStream output = socket.getOutputStream();
			int size = strings.length;
			int i = 0;

			while( i < size ) {
				writeString_( strings[i++]);
			}

			size = xstrings.length;
			i = 0;

			while( i < size ) {
				writeStrings_( xstrings[i++]);
			}
		} catch ( Exception e) {
			e.printStackTrace();
		}

		return object;
	}
}
