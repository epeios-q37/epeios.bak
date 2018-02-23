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

package info.q37.unjsq;

public class UnJSq extends info.q37.xdhq.XDHq {
	private static void launchWeb(String dir) {
		try {
			Runtime runtime = Runtime.getRuntime();
			Process httpd = runtime.exec("node httpd " + dir, null,
					new java.io.File("h:/hg/epeios/tools/xdhq/examples/common/"));
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
			Runtime runtime = Runtime.getRuntime();
			Process electron = runtime.exec(
					"h:/hg/epeios/tools/xdhelcq/node_modules/electron/dist/electron . -m=h:/bin/xdhqxdh h:/hg/epeios/tools/xdhq/examples/common/"
							+ dir,
					null, new java.io.File("h:/hg/epeios/tools/xdhelcq"));
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

	private static final Type defaultType = Type.WEB;

	private static void listen(String newSessionAction, String dir, Type type, String arg) {
		info.q37.xdhq.XDHq.listen(newSessionAction);

		if (type == Type.DEFAULT) {
			type = defaultType;

			if (arg.length() > 0) {
				switch (arg) {
				case "0":
					break;
				case "1":
					type = Type.DESKTOP;
					break;
				case "2":
					type = Type.WEB;
					break;
				case "3":
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

	public static void listen(String newSessionAction, String dir, Type type, String[] args) {
		if (args.length > 0)
			listen(newSessionAction, dir, type, args[0]);
		else
			listen(newSessionAction, dir, type, "");
	}

	public static void listen(String newSessionAction, Type type, String[] args) {
		listen(newSessionAction, ".", type, args);
	}
};
