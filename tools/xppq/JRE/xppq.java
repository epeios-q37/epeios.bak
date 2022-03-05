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

import info.q37.jreq.*;

class XPPqData {
	public String
		tagName,
		attributeName,
		value;
};

class XPPqWrapper extends JRE {
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

class XPPq extends XPPqWrapper {
	public Object core;
	public XPPq( Object core )
	{
		this.core = core;
	}
	
	static public Object getParser( java.io.InputStream stream )
	{
		return call( 0, stream );
	}
	
	public void releaseParser()
	{
		call( 1, core );
	}
	
	public int parse( XPPqData data )
	{
		return ( (java.lang.Integer)call( 2, core, data ) ).intValue();
	}
	
	static public Object getPreprocessor( java.io.InputStream stream )
	{
		return call( 3, stream );
	}
	
	public void releasePreprocessor()
	{
		call( 4, core );
	}
	
	public int readFromPreprocessor()
	{
		return ( (java.lang.Integer)call( 5, core ) ).intValue();
	}

	public void finalize()
	{
	}
}

class XPPqPreprocessor extends java.io.InputStream {
	private XPPq xppq;
	public XPPqPreprocessor( java.io.InputStream Stream )
	{
		xppq = new XPPq( XPPq.getPreprocessor( Stream ) );
	}
	
 // To optimize by using the buffer.
	public int read()
	{
		return xppq.readFromPreprocessor();
	}
	
	public void finalize()
	{
		xppq.releasePreprocessor();
	}
}

class XPPqParser {
	private XPPq xppq;
	// If modified, modify also C++ source file.
	static final int PROCESSED	= 0;
	static final int START_TAG	= 1;
	static final int ATTRIBUTE	= 2;
	static final int VALUE	= 3;
	static final int END_TAG	= 4;
	public XPPqParser( java.io.InputStream Stream )
	{
		xppq = new XPPq( XPPq.getParser( Stream ) );
	}
	
	public int parse( XPPqData data )
	{
//		System.out.println( data.tagName.getClass().getName() );
		return xppq.parse( data  );
	}
	
	public void finalize()
	{
		xppq.releaseParser();
	}
}
