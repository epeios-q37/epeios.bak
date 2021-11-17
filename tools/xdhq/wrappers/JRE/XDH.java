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

package info.q37.xdhq;

import info.q37.xdhq.XDH_SLFH;
import info.q37.xdhq.MODE;

import java.nio.file.*;

public class XDH extends info.q37.jreq.JRE {
	private static MODE mode_ = MODE.UNDEFINED;
	static private String dir_;
	static private String readFile_( String path ) throws java.io.IOException {
		String result = null;

		byte[] encoded = Files.readAllBytes(Paths.get(path));
		return new String(encoded);
	}
	public static boolean isDev() {
		return System.getenv("Q37_EPEIOS") != null;
	}
	private static String getAssetPath_( String dir ) {
		if ( isDev() )
			return "/home/csimon/epeios/tools/xdhq/examples/common/" + dir + "/";
		else
			return "./";
	}
	public static String getAssetFilename_( String path, String dir ) {
		return getAssetPath_( dir ) + path;
	}
	public static String readAsset( String path, String dir ) {
		String content = "";

		try {
			content = readFile_( getAssetFilename_( path, dir ) );
		} catch (java.io.IOException e) {
			e.printStackTrace();
			System.exit(1);
		}

		return content;
	}
	static public void launch(info.q37.xdhq.XDH_SHRD.Callback callback,String headContent, MODE mode, String dir ) {
		mode_ = mode;
		dir_ = dir;

		switch ( mode ) {
		case FAAS:
			XDH_FAAS.launch(headContent,callback );
			break;
		case SLFH:
			XDH_SLFH.launch();
			break;
		default:
			System.err.println( "Unknown mode !!!");
			System.exit(1);
		}
	}
	static public void broadcastAction(String action, String id) {
		switch ( getMode() ) {
		case FAAS:
			XDH_FAAS.broadcastAction(action, id);
			break;
		case SLFH:
			System.err.println( "To implement !!!");
			System.exit(1);
		default:
			System.err.println( "Unknown mode !!!");
			System.exit(1);
		}
	}
	static public MODE getMode() {
		return mode_;
	}
	static public boolean isFaaS() {
		return getMode() == MODE.FAAS;
	}
	static public String getDir() {
		return dir_;
	}
}