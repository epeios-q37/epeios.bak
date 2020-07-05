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
	static private long launcher;
	static {
		launcher = JRE.register( "xppq" );
	}
	static protected Object call(int index, Object... objects) {
		return JRE.call( launcher, index, objects );
	}
	static public String componentInfo(){
		return JRE.componentInfo( launcher );
	}
	static public String returnArgument( String argument )
	{
		return (String)call( 0, argument );
	}
	static public void listen() {
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
		System.out.println( data.tagName.getClass().getName() );
		return xppq.parse( data  );
	}
	
	public void finalize()
	{
		xppq.releaseParser();
	}
}

class XPPqTest {
	private static void displayBytecodeBuildTimestamp() throws Exception
	{
		System.out.println( "Bytecode build : " + new java.util.Date(new java.io.File(XPPqTest.class.getClassLoader().getResource(XPPqTest.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()) );
	}
	
	private static void dump( java.io.InputStream stream ) throws Exception
	{
		int c = 0;
		
		while( ( c = stream.read() ) != -1 ) {
		  System.out.print( (char)c );
		}
		
		System.out.println();
	}
	
	private static void test0( java.io.InputStream stream ) throws Exception
	{
		System.out.println( "No treatment ; to see the original file :" );
		System.out.println( "-----------------------------------------" );
		dump( stream );
	}
	
	private static void test1( java.io.InputStream stream ) throws Exception
	{
		System.out.println( "Preprocessing the file :" );
		System.out.println( "------------------------" );
		dump( new XPPqPreprocessor( stream ) );
	}

	private static void parse( java.io.InputStream stream ) throws Exception
	{
		XPPqData data = new XPPqData();
		XPPqParser parser = new XPPqParser( stream );
		
		int token = parser.parse( data );
		
		while ( token != XPPqParser.PROCESSED ) {
			switch ( token ) {
			case XPPqParser.START_TAG :
				System.out.print( "Start tag: '" + data.tagName + "'\n" );
				break;
			case XPPqParser.ATTRIBUTE :
				System.out.print( "Attribute: '" + data.attributeName + "' = '" + data.value.trim() + "'\n" );
				break;
			case XPPqParser.VALUE :
				System.out.print( "Value:     '" + data.value.trim() + "'\n" );
				break;
			case XPPqParser.END_TAG :
				System.out.print( "End tag:   '" + data.tagName + "'\n" );
				break;
			default:
				throw new Exception( "Unknown token !!!" );
			}
		
			token = parser.parse( data );
		}
	}

	private static void test2( java.io.InputStream stream) throws Exception
	{
		System.out.println( "XML parsing WITHOUT preprocessing :" );
		System.out.println( "-----------------------------------" );
		parse( stream );
	}
 
	private static void test3( java.io.InputStream stream ) throws Exception
	{
		System.out.println( "XML parsing WITH preprocessing :" );
		System.out.println( "--------------------------------" );
//		parse( new XPPqPreprocessor( new java.io.FileInputStream( fileName ) ) );
		parse( new XPPqPreprocessor( stream ) );
	}

 private static String xml = 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
"<SomeTag xmlns:xpp=\"http://q37.info/ns/xpp/\" AnAttribute=\"SomeAttributeValue\">\n" +
" <SomeOtherTag AnotherAttribute=\"AnotherAttributeValue\">TagValue</SomeOtherTag>\n" +
" <xpp:define name=\"SomeMacro\">\n" +
"  <xpp:bloc>Some macro content from string !</xpp:bloc>\n" +
" </xpp:define>\n" +
" <YetAnotherTag YetAnotherAttribute=\"YetAnotherAttributeValue\">\n" +
"  <xpp:expand select=\"SomeMacro\"/>\n" +
" </YetAnotherTag>\n" +
"</SomeTag>\n"
 ;


 private static java.io.InputStream getStream() throws Exception
 {
  if ( false )
   return new java.io.FileInputStream( "demo.xml" );
  else
   return new java.io.ByteArrayInputStream( xml.getBytes() );
 }

	public static void main ( String[] args ) throws Exception
	{
	XPPq.Init();
 	System.out.println( XPPq.wrapperInfo() );
 	System.out.println( XPPq.componentInfo() );
 	displayBytecodeBuildTimestamp();
  System.out.println();
				
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
			test0( getStream() );
			break;
		case 1:
			test1( getStream() );
			break;
		case 2:
			test2( getStream() );
			break;
		case 3:
			test3( getStream() );
			break;
		default:
			System.err.println( "'" + test + "' is not a valid test id ; must be '0' to '3'.");
			System.exit(1);
			break;
		}
	}
}

