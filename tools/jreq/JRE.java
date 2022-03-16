/*
	Copyright (C) 2007 Claude SIMON (http://q37.info/contact/).

	This file is part of JREq.

	JREq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	JREq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with JREq. If not, see <http://www.gnu.org/licenses/>.
*/

// COMPILE WITH OPTION '-d .' : 'javac -d classes *.java'.

package info.q37.jreq;

class Wrapper {
	native public static String wrapperInfo();

	native protected static String componentInfo(long launcher);

	native protected static void init(String location);

	native protected static long register(String arguments);

	native public static Object call(long launcher, int index, Object... objects);

	static private boolean loaded = false;

	static protected java.nio.file.Path getLocation() {
		String osName = System.getProperty("os.name").toLowerCase();

		if (System.getenv("Q37_EPEIOS") == null) {
			return java.nio.file.Paths.get("./");
		} else if (osName.contains("windows"))
			return java.nio.file.Paths.get("h:/bin/");
		else if (osName.contains("mac")) {
			return java.nio.file.Paths.get("/Users/csimon/bin/");
		} else {
			return java.nio.file.Paths.get("/home/csimon/bin/");
		}
	}

	static public void Init() {
		if ( !loaded ) {
			java.nio.file.Path location = getLocation().toAbsolutePath();

			// Don't forget the '-Djava.library.path="…"' parameter to 'java'. 
		//	System.loadLibrary("jreq");
			System.load(java.nio.file.Paths.get(location.toString(), "libjreq.so").toString());

			init(location.toString());

			loaded = true;
		}
	}
}

public class JRE extends Wrapper {
	static public long register( String componentName ) {
		Wrapper.Init();
		return Wrapper.register( java.nio.file.Paths.get(getLocation().toString(), componentName).toString() + "jre" );
	}
}
