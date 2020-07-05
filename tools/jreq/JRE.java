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

	static protected String getLocation() {
		String osName = System.getProperty("os.name").toLowerCase();

		if (System.getenv("Q37_EPEIOS") == null) {
			return "./";
		} else if (osName.contains("windows"))
			return "h:/bin/";
		else if (osName.contains("mac")) {
			return "/Users/csimon/bin/";
		} else {
			return "/home/csimon/bin/";
		}

	}

	static public void Init() {
		if ( !loaded ) {
		String location = getLocation();

		// Don't forget the '-Djava.library.path="…"' parameter to 'java'. 
		System.loadLibrary("jreq");

		init(location);

		loaded = true;
		}
	}
}

public class JRE extends Wrapper {
	static public long register( String componentName ) {
		Wrapper.Init();
		return Wrapper.register( getLocation() + componentName + "jre" );
	}
}
