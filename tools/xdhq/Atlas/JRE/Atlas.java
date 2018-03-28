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

public class Atlas extends info.q37.xdhq.XDHq {
	private static boolean isDev() {
		return System.getenv("EPEIOS_SRC") != null;
	}

	private static void launchWeb(String dir) {
		try {
			String command, path;

			if (isDev()) {
				command = "node httpd ";
				path = "h:/hg/epeios/tools/xdhq/examples/common/";
			} else {
				command = "node -e \"require('xdhwebq').fileName\"";
				path = ".";
			}

			Runtime runtime = Runtime.getRuntime();
			Process httpd = runtime.exec(command + dir, null, new java.io.File(path));
			runtime.addShutdownHook(new Thread() {
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
			String command, path;

			if (isDev()) {
				command = "h:/hg/epeios/tools/xdhelcq/node_modules/electron/dist/electron index.js -m=h:/bin/xdhqxdh h:/hg/epeios/tools/xdhq/examples/common/";
				path = "h:/hg/epeios/tools/xdhelcq";
			} else {
				command = "node -e \"require('child_process').spawnSync(require('xdhelcq').electron,[require('path').join(require('xdhelcq').path, 'index.js'),'-m=' + require('xdhqxdh').fileName, require('upath').normalize(require('path').resolve(__dirname))]);\"";
				path = ".";
			}

			System.out.println( command + " ; " + path);

			Runtime runtime = Runtime.getRuntime();
			Process electron = runtime.exec(command + dir, null, new java.io.File(path));
			runtime.addShutdownHook(new Thread() {
				public void run() {
					electron.destroy();
				}
			});
		} catch (java.io.IOException e) {
			e.printStackTrace();
		}
	}

	public enum Type {
		DEFAULT, DESKTOP, WEB, DESKTOP_AND_WEB
	};

	private static final Type defaultType = Type.DESKTOP;

	private static void launch(String newSessionAction, String dir, Type type, String arg) {
		info.q37.xdhq.XDHq.launch(newSessionAction);

		if (type == Type.DEFAULT) {
			type = defaultType;

			if (arg.length() > 0) {
				switch (arg) {
				case "d":
				case "desktop":
					type = Type.DESKTOP;
					break;
				case "w":
				case "web":
					type = Type.WEB;
					break;
				case "dw":
				case "wd":
					type = Type.DESKTOP_AND_WEB;
					break;
				default:
					System.out.println("Unknown type !");
					System.exit(1);
					break;
				}
			}
		}

		switch (type) {
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
			System.out.println("Unknown type !");
			System.exit(1);
			break;
		}
	}

	public static void launch(String newSessionAction, String dir, Type type, String[] args) {
		if (args.length > 0)
			launch(newSessionAction, dir, type, args[0]);
		else
			launch(newSessionAction, dir, type, "");
	}

	public static void launch(String newSessionAction, Type type) {
		launch(newSessionAction, ".", type, "");
	}
};
