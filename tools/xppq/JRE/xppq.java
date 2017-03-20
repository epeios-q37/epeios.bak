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

class XPPQ extends java.io.InputStream {
	private Object processor;
	private static void DisplayCompilationTime() throws Exception
	{
		System.out.println( new java.util.Date(new java.io.File(XPPQ.class.getClassLoader().getResource(XPPQ.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()) );
	}

	native private static void register();
	native private static Object wrapper(
		int index,
		Object... objects);
		
	private void get( java.io.InputStream Stream )
	{
		processor = wrapper( 0, Stream );
	}
	
	private void release()
	{
		wrapper( 1, processor );
		
		processor = null;
	}
	
	public int read()
	{
		return ((java.lang.Integer)wrapper( 2, processor )).intValue();
	}
	
	public XPPQ( java.io.InputStream Stream )
	{
		get( Stream );
	}

	public static void main ( String[] args ) throws Exception
	{
		DisplayCompilationTime();
		System.out.println( System.getProperty("java.library.path")  );
		System.out.println( System.getProperty("os.arch")  );
		System.loadLibrary("xppqjre");
		register();
		XPPQ xppq = new XPPQ( new java.io.FileInputStream( "demo.xml" ) );
		int c = 0;
		while((c = xppq.read()) != -1) {
		  System.out.print((char)c);
		}
	}
	
	public void finalize()
	{
		release();
	}
}

