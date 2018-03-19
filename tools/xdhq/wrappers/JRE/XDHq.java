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

import info.q37.jreq.JREq;

public class XDHq extends info.q37.jreq.JREq {
	static private long launcher;
	static {
		launcher = JREq.register( "xdhq" );
	}
	static protected Object call(int index, Object... objects) {
		return JREq.call( launcher, index, objects );

	}
	static public String componentInfo(){
		return JREq.componentInfo( launcher );
	}
	static public String returnArgument( String argument )
	{
		return (String)call( 0, argument );
	}
	static public void launch( String newSessionAction ) {
		call( 7, "53752", newSessionAction );
	}
}