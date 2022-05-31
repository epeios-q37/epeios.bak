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

package info.q37.xdhq.dom;

import java.net.*;
import java.io.*;
import java.util.*;
import java.util.concurrent.locks.*;
import java.util.concurrent.*;
import java.awt.Desktop;

public class DOM_FAAS extends DOM_SHRD {
	private static final String VERSION_ = "0.13.1";
	private static String pAddr = "faas.q37.info";
	private static int pPort = 53700;
	private static String wAddr = "";
	private static String wPort = "";
	private static String cgi = "xdh";
	private static String token = "";
	private static InputStream input_;
	private static OutputStream output_;
	// A 'Semaphore' because there are only reentrant locks in Java.
	private static Semaphore readLock_ = new Semaphore(1);	// Global read lock.
	private static final String FAAS_PROTOCOL_LABEL_ = "4c837d30-2eb5-41af-9b3d-6c8bf01d8dbf";
	private static final String FAAS_PROTOCOL_VERSION_ = "1";
	private static final String MAIN_PROTOCOL_LABEL_ = "22bb5d73-924f-473f-a68a-14f41d8bfa83";
	private static final String MAIN_PROTOCOL_VERSION_ = "0";
	private static final String SCRIPTS_VERSION_ = "0";

	private static final int FORBIDDEN_ID_ = -1;
	private static final int CREATION_ID_ = -2;
	private static final int CLOSING_ID_ = -3;
	private static final int HEAD_RETRIEVING_ID_ = -4;
	
	private static final int BROADCAST_ACTION_ID_ = -3;
	private static final int HEAD_SENDING_ID_ = -4;
	
	private int id_;
	private boolean firstLaunch_ =  true;

	static public class ThreadExitingException extends RuntimeException {}

	static class Instance_ {
		private boolean quit = false;
		Semaphore readLock_ = new Semaphore(1);
		String language = null;
		Object userObject;	// Just to avoid that the GC destroys the object.
		Instance_( Object object ) {
			this.userObject = object;
			try {
				readLock_.acquire();
			} catch (InterruptedException e) {
				e.printStackTrace();
				System.exit(1);
			}
		}
		public void waitForData() {
			try {
				readLock_.acquire();
			} catch (InterruptedException e) {
				e.printStackTrace();
				System.exit(1);
			}
			if (quit) {
				instanceDataRead_();
				throw new ThreadExitingException();
			}
		}
		public void dataAvailable() {
			readLock_.release();
		}
	}

	static private Map<Integer, Instance_> instances_ = new HashMap<Integer, Instance_>();

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

	static private boolean isREPLit_() {
		return "REPLIT".equals(getEnv_("ATK").toUpperCase());
	}

	static private boolean isTokenEmpty_() {
		return "".equals(token) || (token.charAt(0) == '&');
	}

	static {
		String atk = getEnv_("ATK").toUpperCase();

		if ("DEV".equals(atk)) {
			pAddr = "localhost";
			wPort = "8080";
			System.out.println("\tDEV mode !");
		} else if ("TEST".equals(atk)) {
			cgi = "xdh_";
			System.out.println("\tTEST mode !");
		} else if (!atk.isEmpty() && !"REPLIT".equals(atk) && !"NONE".equals(atk)) {
			System.err.println("Bad 'ATK' environment variable value : should be 'DEV' or 'TEST' !");
			System.exit(1);
		}

		if ( isTokenEmpty_() ) {
			String token = getEnv_("ATK_TOKEN");

			if ( !"".equals( token ))
				DOM_FAAS.token = "&" + token; 
		}

		pAddr = getEnv_( "ATK_PADDR", pAddr );
		pPort = Integer.parseInt(getEnv_( "ATK_PPORT", String.valueOf(pPort)));
		wAddr = getEnv_( "ATK_WADDR", wAddr );
		wPort = getEnv_( "ATK_WPORT", wPort );

		if ( "".equals(wAddr))
			wAddr = pAddr;

		if (!"".equals(wPort))
			wPort = ":" + wPort;

		try {
			readLock_.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
			System.exit(1);
		}
	}

	private static void writeByte_( byte data) throws IOException {
		output_.write( data );
	}

	private static void writeUInt_(int value) throws IOException {
		byte data[] = new byte[8];
		int i = 7;

		data[i] = (byte) (value & 0x7F);
		value >>= 7;

		while (value != 0) {
			data[--i] = (byte) (value | 0x80);
			value >>= 7;
		}

		output_.write(data, i, 8 - i);
	}

	private static void writeSInt_(int value) throws IOException {
		writeUInt_( value < 0 ? ( ( -value - 1 ) << 1 ) | 1 : value << 1 );
	}


	private static void writeString_(String string) throws IOException {
		byte bytes[] = string.getBytes();

		writeUInt_(bytes.length);
		output_.write(bytes);
	}

	private static void writeStrings_(String[] strings) throws IOException {
		int amount = strings.length;
		int i = 0;

		writeUInt_(amount);

		while (i < amount) {
			writeString_(strings[i++]);
		}
	}

	private static byte getByte_() throws IOException {
		int data = input_.read();

		if ( data == -1 ) {
			System.err.println("EOF !!!");
			System.exit(1);
		}

		return (byte)data;
	}

	private static int getUInt_() throws IOException {
		int datum = input_.read();
		int value = datum & 0x7f;

		while ((datum & 0x80) != 0) {
			datum = input_.read();

			value = (value << 7) + (datum & 0x7f);
		}

		return value;
	}

	private static int getSInt_() throws IOException {
		int value = getUInt_();

		return ( value & 1 ) != 0? -( ( value >> 1 ) + 1 ) : value >> 1;
	}

	private static String getString_() throws IOException {
		String string = "";
		int size = getUInt_();

		while (size-- != 0) {
			string += (char) input_.read();
		}

		return string;
	}

	private static String[] getStrings_() throws IOException {
		int amount = getUInt_();
		int i = 0;

		String[] strings = new String[amount];

		while (i < amount)
			strings[i++] = getString_();

		return strings;
	}

	private static void dismiss_(int id) throws IOException {
		synchronized( output_ ) {
			writeSInt_(id);
			writeString_("#Dismiss_1");
		}
	}

	private static void report_(String message) throws IOException {
		synchronized( output_ ) {
			writeSInt_(-1);
			writeString_("#Inform_1");
			writeString_(message);
		}
	}

	private static void handshakeFaaS_() throws IOException {
		String error, notification;

		writeString_(FAAS_PROTOCOL_LABEL_);
		writeString_(FAAS_PROTOCOL_VERSION_);
		writeString_("JRE " + VERSION_);

		output_.flush();

		error = getString_();

		if ( !error.isEmpty()) {
			System.err.println(error);
			System.exit(1);
		}

		notification = getString_();

		if ( !notification.isEmpty())
			System.out.println(notification );
	}

	private static void handshakeMain_() throws IOException {
		String error, notification;

		writeString_(MAIN_PROTOCOL_LABEL_);
		writeString_(MAIN_PROTOCOL_VERSION_);
		writeString_(SCRIPTS_VERSION_);

		output_.flush();

		error = getString_();

		if ( !error.isEmpty()) {
			System.err.println(error);
			System.exit(1);
		}
	
		notification = getString_();

		if ( !notification.isEmpty())
			System.out.println(notification );
	}

	private static void handshakes_() throws IOException {
		handshakeFaaS_();
		handshakeMain_();
	}

	private static void ignition_() throws IOException {
		writeString_(token);
		// writeString_(info.q37.xdhq.XDH_FAAS.headContent);	// Dedicated request sinve FaaS protocol v1.
		writeString_(wAddr);
		writeString_(""); // Currently not used; for future use.
		
		output_.flush();

		token = getString_();

		if (isTokenEmpty_()) {
			System.err.println(getString_());
			System.exit(1);			
		}
		
		if ( !":0".equals(wPort)) {
			String url = getString_();

			System.out.println(url);
			char[] filler = new char[url.length()];
			Arrays.fill(filler,'^');
			System.out.println(filler);
			System.out.println("Open above URL in a web browser (click, right click or copy/paste). Enjoy!");

			if ( isREPLit_() ) {
				String script =
					"require('http').createServer(function (req, res)" + 
					"{res.end(\"<html><body><iframe style=\\\"border-style: none; width: 100%;height: 100%\\\" " + 
					"src=\\\"https://atlastk.org/repl_it.php?url=" + url + "\\\"</iframe></body></html>\");}).listen(8080);";
				Runtime.getRuntime().exec(new String[] { "/usr/bin/node", "-e", script });
			} else if ("NONE".equals(getEnv_("ATK").toUpperCase())) {
				// Nothing to do.
			} else if (Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.BROWSE)) {
				try {
					Desktop.getDesktop().browse(new URI(url));
				} catch (URISyntaxException e) {
					e.printStackTrace();
					System.exit(1);
				}
			} else if (Runtime.getRuntime().exec(new String[] { "which", "xdg-open" }).getInputStream().read() != -1) {
        Runtime.getRuntime().exec(new String[] { "xdg-open", url });	// For KDE based Linux distros.
      }
		}
	}

	static private void waitForInstance_() {
		try {
			readLock_.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
			System.exit(1);
		}
	}

	static private void instanceDataRead_() {
		readLock_.release();
	}

	private static void serve_(info.q37.xdhq.XDH_SHRD.Callback callback) throws IOException {
		for (;;) {
			int id = getSInt_();

			if ( id == FORBIDDEN_ID_ ) {// Should not happen.
				System.err.println("Received unexpected undefined command id!");
				System.exit(1);				
			} else if ( id == CREATION_ID_ ) {	// New session.
				id = getSInt_();	// The id of the new session.

				if ( instances_.containsKey( id )) {
					report_( "Instance of id  '" + id + "' exists but should not !" );
				}

				synchronized( instances_ ) {
					Object userObject = callback.callback();	//  from here, 'getAction()' could access the instance before being stored, hence the 'synchronized' .

					instances_.put( id, new Instance_( userObject ) );

					try {
						Object object1 = userObject.getClass().getField( "dom" ).get(userObject);

						Object object2 = object1.getClass().getMethod( "getDOM", (Class [])null).invoke( object1 );
					
						object2.getClass().getMethod( "setId", new Class[]{Integer.class}).invoke( object2, id );
					} catch (NoSuchFieldException | IllegalAccessException |  NoSuchMethodException | java.lang.reflect.InvocationTargetException e) {
						e.printStackTrace();
						System.exit(1);						
					}
				}
			} else if ( id == CLOSING_ID_ ) {	// Close session
				id = getSInt_();

				if ( !instances_.containsKey( id ) )
					report_("Instance of id '" + id + "' not available for destruction!");
				else  {
					instances_.get(id).quit = true;
					instances_.get(id).dataAvailable();
					waitForInstance_();
					instances_.remove(id);
				}
			} else if ( id == HEAD_RETRIEVING_ID_ ) {
				writeSInt_(HEAD_SENDING_ID_);
				writeString_(info.q37.xdhq.XDH_FAAS.headContent);
			} else if ( !instances_.containsKey( id ) ) {
					report_( "Unknown instance of id '" + id + "'!" );
					dismiss_(id);
			} else {
				Instance_ instance = instances_.get(id);

				if ( instance.language == null ) {
					instance.language = getString_();
				} else {
					instance.dataAvailable();
					waitForInstance_();
				}
			}
		}
	}

	static public void launch(info.q37.xdhq.XDH_SHRD.Callback callback)  {
		try {
			try {
				Socket socket = new Socket(pAddr, pPort);
				input_ = socket.getInputStream();
				output_ = socket.getOutputStream();
			} catch (UnknownHostException e) {
				System.err.println("Unable to connect to " + pAddr + ":" + pPort + "!");
				System.exit(1);
			}

			handshakes_();

			ignition_();

			serve_(callback);
			instanceDataRead_();
 		} catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		}
	}

	static public void broadcastAction(String action, String id) {
		try {
			synchronized( output_ ) {
				writeSInt_(BROADCAST_ACTION_ID_);
				writeString_(action);
				writeString_(id);
				output_.flush();
			}
 		} catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		}
 	}

	@Override
	public void getAction(Event event) {
		try {
			if (!firstLaunch_) {
				synchronized( output_ ) {
					writeSInt_(id_ );
					writeString_("#StandBy_1");
					output_.flush();
				}
			} else
				firstLaunch_ = false;

			Instance_ instance;

			synchronized(instances_) {	// See 'serve_(...)'.
				instance = instances_.get(id_);
			}

			instance.waitForData();

			event.id = getString_();
			event.action = getString_();

			instanceDataRead_();
 		} catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		}
	}

	@Override
	public Object call(String command, Type type, info.q37.xdhq.ARG ...args) {
		Object object = null;

		try {
			synchronized( output_ ) {
				writeSInt_( id_ );
				writeString_(command);
				writeUInt_(type.getValue());

				for (int i = 0; i < args.length; i++) {
					writeUInt_(args[i].type.getValue());
					if ( args[i].type == Type.STRING) {
						writeString_(args[i].string);
					} else {
						writeStrings_(args[i].strings);
					}
				}

				writeUInt_(Type.VOID.getValue());

				output_.flush();
			}

			switch (type) {
			case VOID:
				break;
			case STRING:
				instances_.get(id_).waitForData();
				object = getString_();
				instanceDataRead_();
				break;
			case STRINGS:
				instances_.get(id_).waitForData();
				object = getStrings_();
				instanceDataRead_();
				break;
			default:
				System.err.println("Unknown return type !!!");
				System.exit(1);
			}
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		}

		return object;
	}

	public void setId( Integer id ) {
		id_ = id;
	}
}
