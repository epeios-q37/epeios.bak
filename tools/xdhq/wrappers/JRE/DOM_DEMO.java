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
import java.util.*;
import java.awt.Desktop;

public class DOM_DEMO extends DOM_SHRD {
	static private String pAddr = "atlastk.org";
	static private int pPort = 53800;
	static private String wAddr = "";
	static private String wPort = "";
	static private String cgi = "xdh";
	static private String token = "";
	private Socket socket_;
	private InputStreamReader reader_;
	private boolean firstLaunch = true;
	static private String demoProtocolLabel = "877c913f-62df-40a1-bf5d-4bb5e66a6dd9";
	static private String demoProtocolVersion = "0";
	static private String mainProtocolLabel = "6e010737-31d8-4be3-9195-c5b5b2a9d5d9";
	static private String mainProtocolVersion = "0";

	class Instance_ {
		byte id;
	}

	Map<Byte, Instance_> instances_ = new HashMap<Byte, Instance_>();

	static private String getEnv_(String name, String value) {
		String env = System.getenv(name);

		if (env == null)
			return value.trim();
		else
			return env.trim();
	}

	static private String getEnv_(String name) {
		return getEnv_(name, "");
	}

	static private boolean isTokenEmpty_() {
		return "".equals(token) || (token.charAt(0) == '&');
	}

	static {
		String atk = getEnv_("ATK");

		if ("DEV".equals(atk)) {
			pAddr = "localhost";
			wPort = "8080";
			System.out.println("\tDEV mode !");
		} else if ("TEST".equals(atk)) {
			cgi = "xdh_";
			System.out.println("\tTEST mode !");
		} else if (!"".equals(atk)) {
			throw new java.lang.RuntimeException("Bad 'ATK' environment variable value : should be 'DEV' or 'TEST' !");
		}

		if ( isTokenEmpty_() ) {
			String token = getEnv_("ATK_TOKEN");

			if ( !"".equals( token ))
				DOM_DEMO.token = "&" + token; 
		}

		pAddr = getEnv_( "ATK_PADDR", pAddr );
		pPort = Integer.parseInt(getEnv_( "ATK_PPORT", String.valueOf(pPort)));
		wAddr = getEnv_( "ATK_WADDR", wAddr );
		wPort = getEnv_( "ATK_WPORT", wPort );

		if ( "".equals(wAddr))
			wAddr = pAddr;

		if (!"".equals(wPort))
			wPort = ":" + wPort;
	}

	private void writeByte_( byte data, OutputStream stream) throws Exception {
		stream.write( data );
	}

	private void writeSize_(int size, OutputStream stream) throws Exception {
		byte data[] = new byte[8];
		int i = 7;

		data[i] = (byte) (size & 0x7F);
		size >>= 7;

		while (size != 0) {
			data[--i] = (byte) (size | 0x80);
			size >>= 7;
		}

		stream.write(data, i, 8 - i);
	}

	private void writeString_(String string, OutputStream stream) throws Exception {
		byte bytes[] = string.getBytes();

		writeSize_(bytes.length, stream);
		stream.write(bytes);
	}

	private void writeString_(String string) throws Exception {
		writeString_(string, socket_.getOutputStream());
	}

	private void writeStrings_(String[] strings, OutputStream stream) throws Exception {
		int size = strings.length;
		int i = 0;

		writeSize_(size, stream);

		while (i < size) {
			writeString_(strings[i++], stream);
		}
	}

	private void writeStrings_(String[] strings) throws Exception {
		writeStrings_(strings, socket_.getOutputStream());
	}

	private byte getByte_() throws Exception {
		int data = reader_.read();

		if ( data == -1 )
			throw new Exception( "EOF !!!");

		return (byte)data;
	}

	private int getSize_() throws Exception {
		int datum = reader_.read();
		int size = datum & 0x7f;

		while ((datum & 0x80) != 0) {
			datum = reader_.read();

			size = (size << 7) + (datum & 0x7f);
		}

		return size;
	}

	private String getString_() throws Exception {
		String string = "";
		int size = getSize_();

		while (size-- != 0) {
			string += (char) reader_.read();
		}

		return string;
	}

	private String[] getStrings_() throws Exception {
		int size = getSize_();
		int i = 0;

		String[] strings = new String[size];

		while (i < size)
			strings[i++] = getString_();

		return strings;
	}

	class Reader implements Runnable
	{
		@Override
		public final void run() {
			try {
				for (;;) {
					byte id = getByte_();

					if ( id == -1 ) {	// Value reporting a new front-end.
						id = getByte_();	// The id of the new front-end.

						if ( instances_.containsKey( id )) {
							throw new Exception( "Instance of id  '" + id + "' exists but should not !" );
						}

						instances_.put( id, new Instance_() );

						synchronized( socket_) {
							OutputStream output = socket_.getOutputStream();

							writeByte_( id, output );
							writeString_( mainProtocolLabel, output );
							writeString_( mainProtocolVersion, output );
					
						}
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	private void ignition_() throws Exception {
		OutputStream output = socket_.getOutputStream();

		writeString_(token, output);

		if (isTokenEmpty_()) {
			writeString_(info.q37.xdhq.XDH_DEMO.headContent);
		}
		
		output.flush();

		token = getString_();

		if (isTokenEmpty_()) {
			System.out.println( getString_() );
			System.exit(1);
		}

		if ( !":0".equals(wPort)) {
			String url = "http://" + wAddr + wPort + "/" + cgi + ".php?_token=" + token;

			System.out.println(url);
			System.out.println("Open above URL in a web browser. Enjoy!");

			if (Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.BROWSE)) {
				Desktop.getDesktop().browse(new URI(url));
			}
		}
	}

	private void demoHandshake_() throws Exception {
		OutputStream output = socket_.getOutputStream();
		String error, notification;

		writeString_( demoProtocolLabel );
		writeString_( demoProtocolVersion );

		error = getString_();

		if ( !error.isEmpty()) {
			System.out.println(error);
			System.exit(1);
		}

		notification = getString_();

		if ( !notification.isEmpty())
			System.out.println(notification );
	}


	public DOM_DEMO() throws Exception {
//		System.out.println( pAddr + ":" + pPort );

		try {
			socket_ = new Socket(pAddr, pPort);
		} catch (Exception e) {
			System.out.println("Unable to connect to " + pAddr + ":" + pPort + " !!!");
			System.exit(1);
		}

		reader_ = new InputStreamReader( socket_.getInputStream() );

		demoHandshake_();

		ignition_();

/*

		writeString_(token, output);

		if (isTokenEmpty_()) {
			writeString_(info.q37.xdhq.XDH_DEMO.headContent);
		}

		output.flush();

		if (isTokenEmpty_()) {
			token = getString_(reader);

			if (isTokenEmpty_()) {
				System.out.println(getString_( reader ));
				System.exit(1);
			}

			if ( !":0".equals(wPort)) {
				String url = "http://" + wAddr + wPort + "/" + cgi + ".php?_token=" + token;

				System.out.println(url);
				System.out.println("Open above URL in a web browser. Enjoy!");

				if (Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.BROWSE)) {
					Desktop.getDesktop().browse(new URI(url));
				}
			}
		} else {
			String returnedToken = getString_(reader);

			if ( returnedToken.equals("")) {
				System.out.println(getString_( reader ));
				System.exit(1);
			}

			if (!returnedToken.equals(token))
				throw new Exception("Unmatched token !!!");
		}

		writeString_(demoProtocolLabel, output);
		writeString_(demoProtocolVersion, output);
		output.flush();

		String errorMessage = getString_(reader);

		if ( !"".equals(errorMessage))
			throw new Exception(errorMessage);

		getString_(reader);	// Language.
		writeString_("JRE");
		output.flush();

		*/
	}

	@Override
	public void getAction(Event event) {
		try {
			if (!firstLaunch) {
				OutputStream output = socket_.getOutputStream();
				output.write(new String("StandBy_1").getBytes());
				output.write(0);
				output.flush();
			} else
				firstLaunch = false;

			event.id = getString_();

			event.action = getString_();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public Object call(String command, Type type, String[] strings, String[][] xstrings) {
		Object object = null;

		try {
			OutputStream output = socket_.getOutputStream();
			int size = strings.length;
			int i = 0;

			output.write(new String(command).getBytes());
			output.write(0);

			while (i < size) {
				writeString_(strings[i++], output);
			}

			size = xstrings.length;
			i = 0;

			while (i < size) {
				writeStrings_(xstrings[i++], output);
			}

			output.flush();

			switch (type) {
			case VOID:
				break;
			case STRING:
				object = getString_();
				break;
			case STRINGS:
				object = getStrings_();
				break;
			default:
				throw new Exception("Unknown return type !!!");
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return object;
	}
}
