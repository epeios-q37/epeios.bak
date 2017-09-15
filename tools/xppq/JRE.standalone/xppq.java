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
	static final int PROCESSED	= 0;
	static final int START_TAG	= 1;
	static final int ATTRIBUTE	= 2;
	static final int VALUE		= 3;
	static final int END_TAG	= 4;
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
	private static void displayCompilationTime() throws Exception
	{
		System.out.println( new java.util.Date(new java.io.File(XPPQDemo.class.getClassLoader().getResource(XPPQDemo.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()) );
	}
	
	private static void dump( java.io.InputStream stream ) throws Exception
	{
		int c = 0;
		
		while( ( c = stream.read() ) != -1 ) {
		  System.out.print( (char)c );
		}
		
		System.out.println();
	}
	
	private static void test0( String fileName ) throws Exception
	{
		System.out.println( "No treatment ; to see the original file :" );
		System.out.println( "-----------------------------------------" );
		dump( new java.io.FileInputStream( fileName ) );
	}
	
	private static void test1( String fileName ) throws Exception
	{
		System.out.println( "Preprocessing the file :" );
		System.out.println( "------------------------" );
		dump( new XPPQPreprocessor( new java.io.FileInputStream( fileName ) ) );
	}
	
	private static void parse( java.io.InputStream stream ) throws Exception
	{
		XPPQData data = new XPPQData();
		XPPQParser parser = new XPPQParser( stream );
		
		int token = parser.parse( data );
		
		while ( token != XPPQParser.PROCESSED ) {
			switch ( token ) {
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

	private static void test2( String fileName ) throws Exception
	{
		System.out.println( "XML parsing WITHOUT preprocessing :" );
		System.out.println( "-----------------------------------" );
		parse( new java.io.FileInputStream( fileName ) );
	}

	private static void test3( String fileName ) throws Exception
	{
		System.out.println( "XML parsing WITH preprocessing :" );
		System.out.println( "--------------------------------" );
		parse( new XPPQPreprocessor( new java.io.FileInputStream( fileName ) ) );
	}

	public static void main ( String[] args ) throws Exception
	{
		System.out.println( XPPQ.info() );
		System.out.println();
		// displayCompilationTime();
		
		int test = 3;
		
		if ( args.length > 0 ) {
			try {
				test = Integer.parseInt( args[0] );
			} catch ( NumberFormatException e) {
				System.err.println( "'" + args[0] + "' is not a valid test id ; must be '0' to '3'.");
				System.exit(1);
			}
		}
		
		String fileName = new String( "demo.xml" );
		
		switch ( test ) {
		case 0:
			test0( fileName );
			break;
		case 1:
			test1( fileName );
			break;
		case 2:
			test2( fileName );
			break;
		case 3:
			test3( fileName );
			break;
		default:
			System.err.println( "'" + args[0] + "' is not a valid test id ; must be '0' to '3'.");
			System.exit(1);
			break;
		}

			
	}
}

