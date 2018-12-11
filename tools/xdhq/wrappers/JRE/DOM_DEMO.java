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
	private Socket socket;
	private boolean firstLaunch = true;
	static private String protocolLabel = "3f0aef6b-b893-4ccd-9316-d468588fc572";
	static private String protocolVersion = "0";

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
		writeString_(string, socket.getOutputStream());
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
		writeStrings_(strings, socket.getOutputStream());
	}

	private int getSize_(InputStreamReader reader) throws Exception {
		int datum = reader.read();
		int size = datum & 0x7f;

		while ((datum & 0x80) != 0) {
			datum = reader.read();

			size = (size << 7) + (datum & 0x7f);
		}

		return size;
	}

	private String getString_(InputStreamReader reader) throws Exception {
		String string = "";
		int size = getSize_(reader);

		while (size-- != 0) {
			string += (char) reader.read();
		}

		return string;
	}

	private String getString_() throws Exception {
		return getString_(new InputStreamReader(socket.getInputStream()));
	}

	private String[] getStrings_(InputStreamReader reader) throws Exception {
		int size = getSize_(reader);
		int i = 0;

		String[] strings = new String[size];

		while (i < size)
			strings[i++] = getString_(reader);

		return strings;
	}

	private String[] getStrings_() throws Exception {
		return getStrings_(new InputStreamReader(socket.getInputStream()));
	}

	public DOM_DEMO() throws Exception {
		try {
			socket = new Socket(pAddr, pPort);
		} catch (Exception e) {
			System.out.println("Unable to connect to " + pAddr + ":" + pPort + " !!!");
			System.exit(1);
		}

		OutputStream output = socket.getOutputStream();
		InputStreamReader reader = new InputStreamReader(socket.getInputStream());

		writeString_(token, output);

		if (isTokenEmpty_()) {
			writeString_(info.q37.xdhq.XDH_DEMO.headContent);
		}

		output.flush();

		if (isTokenEmpty_()) {
			token = getString_(reader);

			if (isTokenEmpty_())
				throw new Exception("Invalid connection information !!!");

			if ( !":0".equals(wPort)) {
				String url = "http://" + wAddr + wPort + "/" + cgi + ".php?_token=" + token;

				System.out.println(url);
				System.out.println("Open above URL in a web browser. Enjoy!");

				if (Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.BROWSE)) {
					Desktop.getDesktop().browse(new URI(url));
				}
			}
		} else {
			if (!getString_(reader).equals(token))
				throw new Exception("Unmatched token !!!");
		}

		writeString_(protocolLabel, output);
		writeString_(protocolVersion, output);
		output.flush();

		String errorMessage = getString_(reader);

		if ( !"".equals(errorMessage))
			throw new Exception(errorMessage);

		getString_(reader);	// Language.
		writeString_("JRE");
		output.flush();
	}

	@Override
	public void getAction(Event event) {
		try {
			if (!firstLaunch) {
				OutputStream output = socket.getOutputStream();
				output.write(new String("StandBy_1").getBytes());
				output.write(0);
				output.flush();
			} else
				firstLaunch = false;

			InputStreamReader reader = new InputStreamReader(socket.getInputStream());

			event.id = getString_(reader);

			event.action = getString_(reader);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public Object call(String command, Type type, String[] strings, String[][] xstrings) {
		Object object = null;

		try {
			OutputStream output = socket.getOutputStream();
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

			InputStreamReader reader = new InputStreamReader(socket.getInputStream());

			switch (type) {
			case VOID:
				break;
			case STRING:
				object = getString_(reader);
				break;
			case STRINGS:
				object = getStrings_(reader);
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
