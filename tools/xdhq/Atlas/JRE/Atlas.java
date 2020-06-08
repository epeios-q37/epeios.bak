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
	along with XDHq. If not, see <http://www.gnu.org/licenses/>.
*/

package info.q37.atlas;

import info.q37.xdhq.MODE;

public abstract class Atlas implements Runnable {
	public DOM dom;

	public Atlas() {
		this.dom = new DOM( info.q37.xdhq.XDH.getMode());
		new Thread( this ).start();
	}

	public abstract void handle(String action, String id );

	@Override
	public final void run() {
		info.q37.xdhq.dom.Event event = new info.q37.xdhq.dom.Event();
		for (;;) {
			dom.getAction(event);

			if (dom.isQuitting() ) {	// MUST be called after 'getAction()…',
										// or the library will hang!
				break;
			}

			handle(event.action, event.id );
		}
		
		// System.out.println("Quitting thread!");
	}

	public static boolean isDev() {
		return info.q37.xdhq.XDH.isDev();
	}

	static public String readAsset( String path, String dir ) {
		return info.q37.xdhq.XDH.readAsset( path, dir );
	}

	public static String readAsset( String path ) {
		return readAsset( path, "" );
	}

	static public XML createXML(String rootTag) {
		return new XML(rootTag);
	}

	static public XML createHTML(String rootTag) {
		return new XML(rootTag);
	}

	static public XML createHTML() {
		return createHTML("");
	}

	private static void launchWeb(String dir) {
		try {
			ProcessBuilder processBuilder;

			if (isDev()) {
				processBuilder = new ProcessBuilder( "node", "httpd", dir );
				processBuilder.directory( new java.io.File("h:/hg/epeios/tools/xdhq/examples/common/" ) );
			} else {
				processBuilder = new ProcessBuilder( "node", "-e", "require(require('xdhwebqnjs').fileName).launch('" + dir + "');");
			}

			final Process httpd = processBuilder.start();
			Runtime.getRuntime().addShutdownHook(new Thread() {
				public void run() {
					httpd.destroy();
				}
			});
		} catch (java.io.IOException e) {
			e.printStackTrace();
		}
	}

	private static void launchDesktop(String dir) {
		try {
			ProcessBuilder processBuilder;

			if (isDev()) {
				processBuilder = new ProcessBuilder( "h:/hg/epeios/tools/xdhelcq/node_modules/electron/dist/electron", "index.js", "-m=h:/bin/xdhqxdh", "h:/hg/epeios/tools/xdhq/examples/common/" + dir);
				processBuilder.directory( new java.io.File("h:/hg/epeios/tools/xdhelcq") );
			} else {
				processBuilder = new ProcessBuilder( "node", "-e", "require('child_process').spawnSync(require('xdhelcq').electron,[require('path').join(require('xdhelcq').path, 'index.js'),'-m=' + require('xdhqxdh').fileName, require('upath').normalize(require('path').join(require('path').resolve(__dirname),'" + dir + "'))])");
			}

			final Process electron = processBuilder.start();
			Runtime.getRuntime().addShutdownHook(new Thread() {
				public void run() {
					electron.destroy();
				}
			});
		} catch (java.io.IOException e) {
			e.printStackTrace();
		}
	}

	private static final GUI getDefaultGUI() {
		if ( isDev() )
			return GUI.NONE;
		else
			return GUI.WEB;
	}

	private static final MODE getDefaultMODE() {
		if ( isDev() )
			return MODE.FAAS;
		else
			return MODE.FAAS;
	}

	private static void launch(info.q37.xdhq.XDH_SHRD.Callback callback, String headContent, String dir, GUI gui, String arg) {
		MODE mode = getDefaultMODE();

		if (gui == GUI.DEFAULT) {
			gui = getDefaultGUI();

			if (arg.length() > 0) {
				if ( arg.equals( "n" ) || arg.equals( "none" ) ) {
					gui = GUI.NONE;
				} else if ( arg.equals( "d" ) || arg.equals( "desktop" ) ) {
					gui = GUI.DESKTOP;
				} else if ( arg.equals( "W" ) ) {
					mode = MODE.FAAS;
					gui = GUI.NONE;	// The httpd server is launched externally.
				} else if ( arg.equals( "w") || arg.equals( "web" ) ) {
					gui = GUI.WEB;
				} else if ( arg.equals( "dw" ) || arg.equals( "wd") ) {
					gui = GUI.DESKTOP_AND_WEB;
				} else {
					System.out.println("Unknown gui $!");
					System.exit(1);
				}
			}
		}

		info.q37.xdhq.XDH.launch(callback, headContent, mode, dir );

		switch (gui) {
		case NONE:
			break;
		case DESKTOP:
			launchDesktop(dir);
			break;
		case WEB:
			launchWeb(dir);
			break;
		case DESKTOP_AND_WEB:
			launchDesktop(dir);
			launchWeb(dir);
			break;
		default:
			System.out.println("Unknown gui !");
			System.exit(1);
			break;
		}
	}

	public static void launch(info.q37.xdhq.XDH_SHRD.Callback callback, String headContent, String dir, GUI gui, String[] args) {
		if (args.length > 0)
			launch(callback, headContent,dir, gui, args[0]);
		else
			launch(callback, headContent, dir, gui, "");
	}

	public static void launch(info.q37.xdhq.XDH_SHRD.Callback callback, String headContent, GUI gui) {
		launch(callback, headContent, ".", gui, "");
	}

	public static void launch(info.q37.xdhq.XDH_SHRD.Callback callback, String headContent) {
		launch(callback, headContent, GUI.DEFAULT);
	}

	public static void launch(info.q37.xdhq.XDH_SHRD.Callback callback) {
		launch(callback, "");
	}

	public static void broadcastAction(String action, String id ) {
		info.q37.xdhq.XDH.broadcastAction(action, id);
	}

	public static void broadcastAction(String action) {
		broadcastAction(action, "");
	}
};
