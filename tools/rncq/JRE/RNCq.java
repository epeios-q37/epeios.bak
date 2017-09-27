/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of RNCq.

	RNCq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	RNCq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with RNCq. If not, see <http://www.gnu.org/licenses/>.
*/

/*
If 'RPN' is 'false', the argument, or, if missing, 'exprALG' must be algebraic ('1 + 2').
if 'RPN' is 'true', the argument, or, if missing, 'exprRPN' must be RPN ('1 2 +').
*/
class Data {
 static String exprALG = "784512.8456120 * 8456120.48651320 - 84651320.896451320 / 84651230.89645120 * 86451320.798465123 / 84651320.789465120 * 12365478985245600000000";
 static String exprRPN = "784512.8456120 8456120.48651320 * 84651320.896451320 - 84651230.89645120 / 86451320.798465123 * 84651320.789465120 / 12365478985245600000000 *";
 static boolean RPN = true;
}

class Decl {
 protected static String affix = "rncq";
}

// Begin of generic part.
class JREq extends Decl {
	native public static String wrapperInfo();
	native public static String componentInfo();
	native private static void init( String location);
	native private static void register( String arguments );
	native protected static Object wrapper(
		int index,
		Object... objects);

	static
	{
  String location = ".";

 	System.loadLibrary( "jreq" );

  if ( System.getenv( "EPEIOS_SRC" ) != null ) {
   if ( System.getProperty("os.name").startsWith( "Windows" ) )
    location = "h:/bin";
   else
    location = "~/bin";
  }

 	init( location );
  register( Decl.affix + "jre");
 }
}
// End of generic part.

class RNCq extends JREq {
	public static String returnArgument( String Text )
	{
		return (String)JREq.wrapper( 0, Text  );
	}

 public static String evaluateALG( String Expression )
 {
  return (String)JREq.wrapper( 1, Expression );
 }

 public static String evaluateRPN( String Expression )
 {
  return (String)JREq.wrapper( 2, Expression );
 }
}

class RNCqTest {
	private static void displayBytecodeBuildTimestamp() throws Exception
	{
		System.out.println( "Bytecode build : " + new java.util.Date(new java.io.File(RNCqTest.class.getClassLoader().getResource(RNCqTest.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()) );
	}

 	public static void main ( String[] args ) throws Exception
	{
 	System.out.println( RNCq.wrapperInfo() );
 	System.out.println( RNCq.componentInfo() );
 	displayBytecodeBuildTimestamp();
 	System.out.println( RNCq.returnArgument( "Text from JAVA file" ) );

  String expr;

  if ( !Data.RPN ) {
   if ( args.length > 0 )
    expr = args[0];
   else
    expr = Data.exprALG;

	   System.out.println( RNCq.evaluateALG( expr ) );
  } else {
   if ( args.length > 0 )
    expr = args[0];
   else
    expr = Data.exprRPN;

   System.out.println( RNCq.evaluateRPN( expr ) );
  }
	}
}

