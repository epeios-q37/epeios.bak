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

class XPPQ {
	native private static void register();
	native private static Object wrapper(
		int index,
		Object... objects);
		
	static
	{
		System.loadLibrary("xppqjre");
		register();
	}
	
	public Object getPreprocessor( java.io.InputStream stream )
	{
		return wrapper( 0, stream );
	}
	
	public void releasePreprocessor( Object object )
	{
		wrapper( 1, object );
	}
	
	public int readFromPreprocessor( Object object )
	{
		return ((java.lang.Integer)wrapper( 2, object ) ).intValue();
	}

	public void finalize()
	{
	}
}

class XPPQPreprocessor extends java.io.InputStream {
	private XPPQ xppq;
	private Object processor;
	public XPPQPreprocessor( java.io.InputStream Stream )
	{
		xppq = new XPPQ();
		processor = xppq.getPreprocessor( Stream );
	}
	
	public int read()
	{
		return xppq.readFromPreprocessor( processor );
	}
	
	public void finalize()
	{
		xppq.releasePreprocessor( processor );
	}
}

class XPPQDemo {
	private static void DisplayCompilationTime() throws Exception
	{
		System.out.println( new java.util.Date(new java.io.File(XPPQDemo.class.getClassLoader().getResource(XPPQDemo.class.getCanonicalName().replace('.', '/') + ".class").toURI()).lastModified()) );
	}

	public static void main ( String[] args ) throws Exception
	{
		DisplayCompilationTime();
		XPPQPreprocessor xppq = new XPPQPreprocessor( new java.io.FileInputStream( "demo.xml" ) );
		int c = 0;
		while((c = xppq.read()) != -1) {
		  System.out.print((char)c);
		}
	}
}

