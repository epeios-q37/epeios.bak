/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
*/ 

package info.q37.xppq;

import info.q37.jreq.JRE;

public class XPPqWrapper extends JRE {
	private static long launcher;
	static {
		launcher = JRE.register( "xppq" );
	}
	protected static Object call(int index, Object... objects) {
		return JRE.call( launcher, index, objects );
	}
	public static String componentInfo(){
		return JRE.componentInfo( launcher );
	}
	public static String returnArgument( String argument )
	{
		return (String)call( 0, argument );
	}
	public static void listen() {
		call( 7, "53752");
	}
}
