/*
	'XMLPreprocessorDemo.java' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor library for 'XPPInputStream' JAVA component.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    'XMLPreprocessorDemo.java' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XMLPreprocessorDemo.java' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'XMLPreprocessorDemo.java'.  If not, see <http://www.gnu.org/licenses/>.
*/

import java.io.*;

class XPPQ {
	private static void DisplayCompilationTime() throws Exception
	{
		XPPQ c=new XPPQ();
		System.out.println( new java.util.Date(new File(c.getClass().getClassLoader().getResource(c.getClass().getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()) );
	}

	native private static void register();
	native private static void wrapper(
		int index,
		Object... objects);
		
	private void test0()
	{
		wrapper( 0, this );
	}
	
	private void test1()
	{
		wrapper( 1, this );
	}
	
	public static void main ( String[] args ) throws Exception
	{
		DisplayCompilationTime();
		System.loadLibrary("x64/Debug/xppqjre");
		register();
		XPPQ xppq = new XPPQ();
		xppq.test0();
		xppq.test1();
	}
}

