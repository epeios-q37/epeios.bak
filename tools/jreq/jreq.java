/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of JREq.

	JREq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	JREq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with JREq. If not, see <http://www.gnu.org/licenses/>.
*/

class JREqDecl {
 protected static String affix = ""; // We are not interested in a component.
}


// Begin of generic part.
class JREq extends JREqDecl {
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
//  register( "./" + JREqDecl.affix + "jre");
  }
}
// End of generic part.


class JREqTest {
	private static void displayCompilationTime() throws Exception
	{
		System.out.println( "Bytecode build : " + new java.util.Date(new java.io.File(JREqTest.class.getClassLoader().getResource(JREqTest.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()) );
	}
	
	public static void main ( String[] args ) throws Exception
	{
 	System.out.println( JREq.wrapperInfo() );
 	displayCompilationTime();
 }
}

