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
import java.util.concurrent.locks.*;
import java.awt.Desktop;

public class DOM_FAAS extends DOM_SHRD {
	static private String pAddr = "faas1.q37.info";
	static private int pPort = 53700;
	static private String wAddr = "";
	static private String wPort = "";
	static private String cgi = "xdh";
	static private String token = "";
	static private InputStream input_;
	static private OutputStream output_;
	// Both object are to block the switcher.
	static private Lock lock_ = new ReentrantLock();
	static private Condition condition_ = lock_.newCondition();
	static private String FaaSProtocolLabel = "9efcf0d1-92a4-4e88-86bf-38ce18ca2894";
	static private String FaaSProtocolVersion = "0";
	static private String mainProtocolLabel = "bf077e9f-baca-48a1-bd3f-bf5181a78666";
	static private String mainProtocolVersion = "0";

	private int id_;
	private boolean firstLaunch_ =  true;

	static class Instance_ {
		private boolean handshakeDone = false;
		private boolean quit = false;
		Lock lock = new ReentrantLock();
		Condition condition = lock.newCondition();
		Object userObject;	// Just to avoid that the GC destroys the object.
		Instance_( Object object ) {
			this.userObject = object;
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
		return "REPLit".equals(getEnv_("ATK"));
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
		} else if (!atk.isEmpty() && !"REPLit".equals(atk)) {
			throw new java.lang.RuntimeException("Bad 'ATK' environment variable value : should be 'DEV' or 'TEST' !");
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
	}

	private static void writeByte_( byte data) throws Exception {
		output_.write( data );
	}

	private static void writeUInt_(int value) throws Exception {
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

	private static void writeSInt_(int value) throws Exception {
		writeUInt_( value < 0 ? ( ( -value - 1 ) << 1 ) | 1 : value << 1 );
	}


	private static void writeString_(String string) throws Exception {
		byte bytes[] = string.getBytes();

		writeUInt_(bytes.length);
		output_.write(bytes);
	}

	private static void writeStrings_(String[] strings) throws Exception {
		int amount = strings.length;
		int i = 0;

		writeUInt_(amount);

		while (i < amount) {
			writeString_(strings[i++]);
		}
	}

	private static byte getByte_() throws Exception {
		int data = input_.read();

		if ( data == -1 )
			throw new Exception( "EOF !!!");

		return (byte)data;
	}

	private static int getUInt_() throws Exception {
		int datum = input_.read();
		int value = datum & 0x7f;

		while ((datum & 0x80) != 0) {
			datum = input_.read();

			value = (value << 7) + (datum & 0x7f);
		}

		return value;
	}

	private static int getSInt_() throws Exception {
		int value = getUInt_();

		return ( value & 1 ) != 0? -( ( value >> 1 ) + 1 ) : value >> 1;
	}

	private static String getString_() throws Exception {
		String string = "";
		int size = getUInt_();

		while (size-- != 0) {
			string += (char) input_.read();
		}

		return string;
	}

	private static String[] getStrings_() throws Exception {
		int amount = getUInt_();
		int i = 0;

		String[] strings = new String[amount];

		while (i < amount)
			strings[i++] = getString_();

		return strings;
	}

	private static void FaaSHandshake_() throws Exception {
		String error, notification;

		writeString_( FaaSProtocolLabel );
		writeString_( FaaSProtocolVersion );

		output_.flush();

		error = getString_();

		if ( !error.isEmpty()) {
			throw new RuntimeException(error);
		}

		notification = getString_();

		if ( !notification.isEmpty())
			System.out.println(notification );
	}

	private static void ignition_() throws Exception {
		writeString_(token);
		writeString_(info.q37.xdhq.XDH_FAAS.headContent);
		writeString_(wAddr);
		writeString_("JRE");
		
		output_.flush();

		token = getString_();

		if (isTokenEmpty_()) {
			throw new RuntimeException( getString_() );
		}
		
		if ( !":0".equals(wPort)) {
//			String url = "http://" + wAddr + wPort + "/" + cgi + ".php?_token=" + token;
			String url = getString_();

			System.out.println(url);
			char[] filler = new char[url.length()];
			Arrays.fill(filler,'^');
			System.out.println(filler);
			System.out.println("Open above URL in a web browser. Enjoy!");

			if ( isREPLit_() ) {
				String script =
					"require('http').createServer(function (req, res)" + 
					"{res.end(\"<html><body><iframe style=\\\"border-style: none; width: 100%;height: 100%\\\" " + 
					"src=\\\"https://atlastk.org/repl_it.php?url=" + url + "\\\"</iframe></body></html>\");}).listen(8080);";
				Runtime.getRuntime().exec(new String[] { "/usr/bin/node", "-e", script });
			} else if (Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.BROWSE)) {
				Desktop.getDesktop().browse(new URI(url));
			} else if (Runtime.getRuntime().exec(new String[] { "which", "xdg-open" }).getInputStream().read() != -1) {
                Runtime.getRuntime().exec(new String[] { "xdg-open", url });	// For KDE based Linux distros.
            }
		}
	}

	 private static void serve_(info.q37.xdhq.XDH_SHRD.Callback callback) {
		try {
			for (;;) {
				int id = getSInt_();

				if ( id == -1 ) // Should not happen.
					throw new Exception("Received unexpected undefined command id!");
				else if ( id == -2 ) {	// New session.
					id = getSInt_();	// The id of the new session.

					if ( instances_.containsKey( id )) {
						throw new Exception( "Instance of id  '" + id + "' exists but should not !" );
					}

					synchronized( instances_ ) {
						Object userObject = callback.callback();	//  from here, 'getAction()' could access the instance before being stored, hence the 'synchronized' .

						instances_.put( id, new Instance_( userObject ) );

						Object object1 = userObject.getClass().getField( "dom" ).get(userObject);

						Object object2 = object1.getClass().getMethod( "getDOM", (Class [])null).invoke( object1 );
					
						object2.getClass().getMethod( "setId", new Class[]{Integer.class}).invoke( object2, id );
					}

					synchronized( output_) {
						writeSInt_( id );
						writeString_( mainProtocolLabel );
						writeString_( mainProtocolVersion );
						output_.flush();
					}
				} else if ( id == -3 ) {	// Close session
					id = getSInt_();

					if ( !instances_.containsKey( id ) )
						throw new Exception("Instance of id '" + id + "' not available for destruction!");

					instances_.get(id).quit = true;

					instances_.get(id).lock.lock();
					instances_.get(id).condition.signal();
					instances_.get(id).lock.unlock();

					lock_.lock();
					condition_.await();
					lock_.unlock();

					instances_.remove(id);
				} else if ( !instances_.containsKey( id ) ) {
						throw new Exception( "Unknown instance of id '" + id + "'!" );
				} else if ( !instances_.get(id).handshakeDone ) {
						String error;

						error = getString_();

						if ( !error.isEmpty() ) {
							throw new Exception( error );
						}
					
						getString_();	// Language. Ignored yet.

						instances_.get(id).handshakeDone = true;
				} else {
					instances_.get(id).lock.lock();
					instances_.get(id).condition.signal();
					instances_.get(id).lock.unlock();

					lock_.lock();
					condition_.await();
					lock_.unlock();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	static public void launch(info.q37.xdhq.XDH_SHRD.Callback callback) {
		try {
			try {
				Socket socket = new Socket(pAddr, pPort);
				input_ = socket.getInputStream();
				output_ = socket.getOutputStream();
			} catch (Exception e) {
				System.out.println("Unable to connect to " + pAddr + ":" + pPort + " !!!");
				System.exit(1);
			}

			FaaSHandshake_();

			ignition_();

			serve_(callback);

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	static public void broadcastAction(String action, String id)
	{
		try {
			synchronized( output_ ) {
				writeSInt_(-3);
				writeString_(action);
				writeString_(id);
				output_.flush();
			}
		} catch (Exception e) {
			e.printStackTrace();
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

			instance.lock.lock();
			instance.condition.await();

			if ( !instance.quit ) {
				event.id = getString_();
				event.action = getString_();
			}

			instance.lock.unlock();

			// Some unlocking is done by below 'isQuitting(…)' method,
			// which MUST be called or the library will hang! 
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override public boolean isQuitting() {
		boolean answer = instances_.get(id_).quit; 

		// Below three line were in 'getAction()', but
		// in case of 'quit' being at 'true', there was
		// a risk that 'instances_.get(id_)' were already
		// destroyed, hence an error on above line.
		lock_.lock();
		condition_.signal();
		lock_.unlock();

		return answer;
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
				instances_.get(id_).lock.lock();
				instances_.get(id_).condition.await();
				object = getString_();
				instances_.get(id_).lock.unlock();
				lock_.lock();
				condition_.signal();
				lock_.unlock();
				break;
			case STRINGS:
				instances_.get(id_).lock.lock();
				instances_.get(id_).condition.await();
				object = getStrings_();
				instances_.get(id_).lock.unlock();
				lock_.lock();
				condition_.signal();
				lock_.unlock();
				break;
			default:
				throw new Exception("Unknown return type !!!");
			}



		} catch (Exception e) {
			e.printStackTrace();
		}

		return object;
	}

	public void setId( Integer id ) {
		id_ = id;
	}
}
