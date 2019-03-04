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

public class DOM_DEMO extends DOM_SHRD {
	static private String pAddr = "atlastk.org";
	static private int pPort = 53800;
	static private String wAddr = "";
	static private String wPort = "";
	static private String cgi = "xdh";
	static private String token = "";
	static private InputStream input_;
	static private OutputStream output_;
	// Both object are to block the switcher.
	static private Lock lock_ = new ReentrantLock();
	static private Condition condition_ = lock_.newCondition();;
	static private String demoProtocolLabel = "877c913f-62df-40a1-bf5d-4bb5e66a6dd9";
	static private String demoProtocolVersion = "0";
	static private String mainProtocolLabel = "6e010737-31d8-4be3-9195-c5b5b2a9d5d9";
	static private String mainProtocolVersion = "0";

	private byte id_;
	private boolean firstLaunch_ =  true;

	static class Instance_ {
		private boolean handshakeDone = false;
		Lock lock = new ReentrantLock();
		Condition condition = lock.newCondition();
		Object userObject;	// Just to avoid that the GC destroys the object.
		Instance_( Object object ) {
			this.userObject = object;
		}
	}

	static private Map<Byte, Instance_> instances_ = new HashMap<Byte, Instance_>();

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

	private static void writeByte_( byte data) throws Exception {
		output_.write( data );
	}

	private static void writeSize_(int size) throws Exception {
		byte data[] = new byte[8];
		int i = 7;

		data[i] = (byte) (size & 0x7F);
		size >>= 7;

		while (size != 0) {
			data[--i] = (byte) (size | 0x80);
			size >>= 7;
		}

		output_.write(data, i, 8 - i);
	}

	private static void writeString_(String string) throws Exception {
		byte bytes[] = string.getBytes();

		writeSize_(bytes.length);
		output_.write(bytes);
	}

	private static void writeStrings_(String[] strings) throws Exception {
		int size = strings.length;
		int i = 0;

		writeSize_(size);

		while (i < size) {
			writeString_(strings[i++]);
		}
	}

	private static byte getByte_() throws Exception {
		int data = input_.read();

		if ( data == -1 )
			throw new Exception( "EOF !!!");

		return (byte)data;
	}

	private static int getSize_() throws Exception {
		int datum = input_.read();
		int size = datum & 0x7f;

		while ((datum & 0x80) != 0) {
			datum = input_.read();

			size = (size << 7) + (datum & 0x7f);
		}

		return size;
	}

	private static String getString_() throws Exception {
		String string = "";
		int size = getSize_();

		while (size-- != 0) {
			string += (char) input_.read();
		}

		return string;
	}

	private static String[] getStrings_() throws Exception {
		int size = getSize_();
		int i = 0;

		String[] strings = new String[size];

		while (i < size)
			strings[i++] = getString_();

		return strings;
	}

	private static void demoHandshake_() throws Exception {
		String error, notification;

		writeString_( demoProtocolLabel );
		writeString_( demoProtocolVersion );

		output_.flush();

		error = getString_();

		if ( !error.isEmpty()) {
			System.out.println(error);
			System.exit(1);
		}

		notification = getString_();

		if ( !notification.isEmpty())
			System.out.println(notification );
	}

	private static void ignition_() throws Exception {
		writeString_(token);

		writeString_(info.q37.xdhq.XDH_DEMO.headContent);
		
		output_.flush();

		token = getString_();

		if (isTokenEmpty_()) {
			System.out.println( getString_() );
			System.exit(1);
		}

		if ( !":0".equals(wPort)) {
			String url = "http://" + wAddr + wPort + "/" + cgi + ".php?_token=" + token;

			System.out.println(url);
			char[] filler = new char[url.length()];
			Arrays.fill(filler,'^');
			System.out.println(filler);
			System.out.println("Open above URL in a web browser. Enjoy!");

			if (Desktop.isDesktopSupported() && Desktop.getDesktop().isSupported(Desktop.Action.BROWSE)) {
				Desktop.getDesktop().browse(new URI(url));
			}
		}
	}

	 private static void serve_(info.q37.xdhq.XDH_SHRD.Callback callback) {
		try {
			for (;;) {
				byte id = getByte_();

				if ( id == -1 ) {	// Value reporting a new front-end.
					id = getByte_();	// The id of the new front-end.

					if ( instances_.containsKey( id )) {
						throw new Exception( "Instance of id  '" + id + "' exists but should not !" );
					}

					synchronized( instances_ ) {
						Object userObject = callback.callback();	//  from here, 'getAction()' could access the instance before being stored, hence the 'synchronized' .

						instances_.put( id, new Instance_( userObject ) );

						Object object1 = userObject.getClass().getField( "dom" ).get(userObject);

						Object object2 = object1.getClass().getMethod( "getDOM", (Class [])null).invoke( object1 );
					
						object2.getClass().getMethod( "setId", new Class[]{Byte.class}).invoke( object2, id );
					}

					synchronized( output_) {
						writeByte_( id );
						writeString_( mainProtocolLabel );
						writeString_( mainProtocolVersion );
						output_.flush();
					}
				} else if ( !instances_.containsKey( id ) ) {
						System.out.println( "Unknown instance of id '" + id + "'!" );
						System.exit( -1 );
				} else if ( !instances_.get(id).handshakeDone ) {
						String error;

						error = getString_();

						if ( !error.isEmpty() ) {
							System.out.println( error );
							System.exit( -1 );
						}
					
						getString_();	// Language. Ignored yet.

						synchronized( output_) {
							writeByte_( id );
							writeString_("JRE");
							output_.flush();
						}

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

			demoHandshake_();

			ignition_();

			serve_(callback);

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public void getAction(Event event) {
		try {
			if (!firstLaunch_) {
				synchronized( output_ ) {
					writeByte_(id_ );
					output_.write(new String("StandBy_1").getBytes());
					output_.write(0);
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

			event.id = getString_();

			event.action = getString_();

			instance.lock.unlock();

			lock_.lock();
			condition_.signal();
			lock_.unlock();

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public Object call(String command, Type type, String[] strings, String[][] xstrings) {
		Object object = null;

		try {
			synchronized( output_ ) {
				int size = strings.length;
				int i = 0;

				writeByte_( id_ );
				output_.write(new String(command).getBytes());
				output_.write(0);

				while (i < size) {
					writeString_(strings[i++]);
				}

				size = xstrings.length;
				i = 0;

				while (i < size) {
					writeStrings_(xstrings[i++]);
				}

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

	public void setId( Byte id ) {
		id_ = id;
	}
}
