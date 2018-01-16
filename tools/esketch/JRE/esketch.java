/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of eSketch.

	eSketch is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	eSketch is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with eSketch. If not, see <http://www.gnu.org/licenses/>.
*/

import info.q37.jreq.JREq;

public class eSketch extends info.q37.jreq.JREq {
	static private long launcher;
	static {
		launcher = JREq.register( "esketch" );
	}
	static private Object wrap(int index, Object... objects) {
		return JREq.wrap( launcher, index, objects );
	}
	static public String componentInfo(){
		return JREq.componentInfo( launcher );
	}
	static public String returnArgument( String argument )
	{
		return (String)wrap( 0, argument );
	}
	static public String [] testStrings( String strings[])
	{
		return (String [])wrap( 1, (Object)strings );
	}
}

class eSketchTest {
	private static void display( String[] Strings )
	{
		int index = 0;

		while ( index < Strings.length ) {
			System.out.print( Strings[index++]);
		}
	}
	private static void displayBytecodeBuildTimestamp() throws Exception
	{
		System.out.println( "Bytecode build : " + new java.util.Date(new java.io.File(eSketchTest.class.getClassLoader().getResource(eSketchTest.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()) );
	}
	
	public static void main ( String[] args ) throws Exception
	{
 	System.out.println( eSketch.wrapperInfo() );
 	System.out.println( eSketch.componentInfo() );
 	displayBytecodeBuildTimestamp();
 	System.out.println( eSketch.returnArgument( "Text from JAVA file" ) );
	display(eSketch.testStrings( new String[] {"Hello ", "the ", "world !"}));
	}
}

