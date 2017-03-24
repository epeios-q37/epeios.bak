/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of xppq.

	xppq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xppq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xppq. If not, see <http://www.gnu.org/licenses/>.
*/

class XPPQData {
	public String
		tagName,
		attributeName,
		value;
};

class XPPQ {
	native public static String info();
	native private static void register();
	native private static Object wrapper(
		int index,
		Object... objects);
		
	public Object core;
	
	static
	{
		System.loadLibrary("xppqjre");
		register();
	}
	
	public XPPQ( Object core )
	{
		this.core = core;
	}
	
	static public Object getPreprocessor( java.io.InputStream stream )
	{
		return wrapper( 0, stream );
	}
	
	public void releasePreprocessor()
	{
		wrapper( 1, core );
	}
	
	public int readFromPreprocessor()
	{
		return ( (java.lang.Integer)wrapper( 2, core ) ).intValue();
	}
	
	static public Object getParser( java.io.InputStream stream )
	{
		return wrapper( 3, stream );
	}
	
	public void releaseParser()
	{
		wrapper( 4, core );
	}
	
	public int parse( XPPQData data )
	{
		return ( (java.lang.Integer)wrapper( 5, core, data ) ).intValue();
	}
	
	public void finalize()
	{
	}
}

class XPPQPreprocessor extends java.io.InputStream {
	private XPPQ xppq;
	public XPPQPreprocessor( java.io.InputStream Stream )
	{
		xppq = new XPPQ( XPPQ.getPreprocessor( Stream ) );
	}
	
	public int read()
	{
		return xppq.readFromPreprocessor();
	}
	
	public void finalize()
	{
		xppq.releasePreprocessor();
	}
}

class XPPQParser {
	private XPPQ xppq;
	// If modified, modify also C++ source file.
	static final int ERROR		= 0;
	static final int PROCESSED	= 1;
	static final int START_TAG	= 2;
	static final int ATTRIBUTE	= 3;
	static final int VALUE		= 4;
	static final int END_TAG	= 5;
	public XPPQParser( java.io.InputStream Stream )
	{
		xppq = new XPPQ( XPPQ.getParser( Stream ) );
	}
	
	public int parse( XPPQData data )
	{
		return xppq.parse( data  );
	}
	
	public void finalize()
	{
		xppq.releaseParser();
	}
}

class XPPQDemo {
	private static void DisplayCompilationTime() throws Exception
	{
		System.out.println( new java.util.Date(new java.io.File(XPPQDemo.class.getClassLoader().getResource(XPPQDemo.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()) );
	}

	public static void main ( String[] args ) throws Exception
	{
		System.out.println( XPPQ.info() );
		DisplayCompilationTime();
		XPPQPreprocessor xppq = new XPPQPreprocessor( new java.io.FileInputStream( "demo.xml" ) );
		int c = 0;
		while((c = xppq.read()) != -1) {
		  System.out.print((char)c);
		}
		
		System.out.println();
		
		XPPQData data = new XPPQData();
		XPPQParser parser = new XPPQParser( new XPPQPreprocessor( new java.io.FileInputStream( "demo.xml" ) ) );
		
		int token = parser.parse( data );
		
		while ( token != XPPQParser.PROCESSED ) {
			switch ( token ) {
			case XPPQParser.ERROR :
				throw new Exception( data.value );
			case XPPQParser.START_TAG :
				System.out.print( "Start tag: '" + data.tagName + "'\n" );
				break;
			case XPPQParser.ATTRIBUTE :
				System.out.print( "Attribute: '" + data.attributeName + "' = '" + data.value + "'\n" );
				break;
			case XPPQParser.VALUE :
				System.out.print( "Value:     '" + data.value.trim() + "'\n" );
				break;
			case XPPQParser.END_TAG :
				System.out.print( "End tag:   '" + data.tagName + "'\n" );
				break;
			default:
				throw new Exception( "Unknown token !!!" );
			}
		
			token = parser.parse( data );
		}
	}
}

