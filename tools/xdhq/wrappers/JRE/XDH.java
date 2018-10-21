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

package info.q37.xdhq;

import info.q37.xdhq.XDH_PROD;
import info.q37.xdhq.MODE;

import java.nio.file.*;

public class XDH extends info.q37.jreq.JRE {
	private static MODE mode_ = MODE.UNDEFINED;
	static private String dir_;
	static private String readFile_( String path ) {
		String result = null;
		try {
			  byte[] encoded = Files.readAllBytes(Paths.get(path));
			  result = new String(encoded);
		  } catch ( Exception e) {
				System.out.println(e.getStackTrace());
		  }

		  return result;
	}
	public static boolean isDev() {
		return System.getenv("EPEIOS_SRC") != null;
	}
	private static String getAssetPath_( String dir ) {
		if ( isDev() )
			return "h:/hg/epeios/tools/xdhq/examples/common/" + dir + "/";
		else
			return "./";
	}
	public static String getAssetFilename_( String path, String dir ) {
		return getAssetPath_( dir ) + path;
	}
	public static String readAsset( String path, String dir ) {
		return readFile_( getAssetFilename_( path, dir ) );
	}
	static public void launch( String newSessionAction, String headContent, MODE mode, String dir ) {
		mode_ = mode;
		dir_ = dir;

		switch ( mode ) {
		case DEMO:
			XDH_DEMO.launch( newSessionAction, headContent );
			break;
		case PROD:
			XDH_PROD.launch( newSessionAction );
			break;
		default:
			throw new RuntimeException( "Unknown mode !!!");
		}
	}
	static public MODE getMode() {
		return mode_;
	}
	static public boolean isDEMO() {
		return getMode() == MODE.DEMO;
	}
	static public String getDir() {
		return dir_;
	}
}