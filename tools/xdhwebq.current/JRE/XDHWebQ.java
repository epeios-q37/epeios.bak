/*
	Copyright (C) 2015 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHWebQ.

	XDHWebQ is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHWebQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHWebQ. If not, see <http://www.gnu.org/licenses/>.
*/

class JREqDecl {
 protected static String affix = "xdhwebq";
}

// Begin of generic part.
class JREq extends JREqDecl {
	static long launcher = 0;
	native public static String wrapperInfo();
	native public static String componentInfo( long launcher );
	native private static void init( String location);
	native private static long register( String arguments );
	native protected static Object wrapper(
		long launcher,
		int index,
		Object... objects);

	static
	{
  String location = "";
  String osName = System.getProperty("os.name").toLowerCase();

 	System.loadLibrary( "jreq" );

  if ( osName.contains( "windows" ) )
   location = "h:/bin";
  else if ( osName.contains( "mac" ) ) {
   location = "/Users/bin";
  } else {
   location = "/home/csimon/bin";
  }

  if ( System.getenv( "EPEIOS_SRC" ) == null ) {
   location = ".";
  }

 	init( location );
  launcher = register( "./" + JREqDecl.affix + "jre");
 }
}
// End of generic part.

class XDHWebQ extends JREq {
	public static String returnArgument( String Text )
	{
		return (String)JREq.wrapper( launcher, 0, Text  );
	}
}

class XDHWebQTest {
	private static void displayBytecodeBuildTimestamp() throws Exception
	{
		System.out.println( "Bytecode build : " + new java.util.Date(new java.io.File(XDHWebQTest.class.getClassLoader().getResource(XDHWebQTest.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()) );
	}
	
	public static void main ( String[] args ) throws Exception
	{
 	System.out.println( XDHWebQ.wrapperInfo() );
 	System.out.println( XDHWebQ.componentInfo( JREq.launcher) );
 	displayBytecodeBuildTimestamp();
 	System.out.println( XDHWebQ.returnArgument( "Text from JAVA file" ) );
	}
}

