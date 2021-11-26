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
		try {
			for (;;) {
				dom.getAction(event);

				if ( "".equals(event.action) && isDev() )
					dom.debug_log();

				handle(event.action, event.id);
			}
		} catch (info.q37.xdhq.dom.DOM_FAAS.ThreadExitingException e) {}
		
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
			System.exit(1);
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
			System.exit(1);
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
					System.err.println("Unknown gui !");
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
			System.err.println("Unknown gui !");
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
