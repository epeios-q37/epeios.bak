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

import info.q37.jreq.Core;

public class XPPq extends XPPqWrapper {
	public Core core;
	public XPPq(long pointer)
	{
		core = new Core(pointer);
	}
	
	public static long getParser( java.io.InputStream stream )
	{
		return ( (java.lang.Long)call( 11, stream )).longValue();
	}
	
	public int parse( XPPqData data )
	{
		return ( (java.lang.Integer)call( 12, core, data ) ).intValue();
	}
	
	public static long getPreprocessor( java.io.InputStream stream )
	{
		return ( (java.lang.Long)call( 21, stream )).longValue();
	}
	
	public int readFromPreprocessor()
	{
		return ( (java.lang.Integer)call( 22, core ) ).intValue();
	}

	public int readFromPreprocessor(byte[] b, int off, int len) 
	{
		// Not implemented yet (see C++ source) !
		return ( (java.lang.Integer)call( 23, core, b, off, len ) ).intValue();		
	}	
}

