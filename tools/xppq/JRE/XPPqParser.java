/*
	Copyright (C) 2007 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq software.

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/ 

package info.q37.xppq;

public class XPPqParser {
	private XPPq xppq;
	// If modified, modify also C++ source file.
	public static final int DONE	= 0;
	public static final int START_TAG	= 1;
	public static final int ATTRIBUTE	= 2;
	public static final int VALUE	= 3;
	public static final int END_TAG	= 4;
	public XPPqParser( java.io.InputStream Stream )
	{
		xppq = new XPPq( XPPq.getParser( Stream ) );
	}
	
	public int parse( XPPqData data )
	{
//		System.out.println( data.tagName.getClass().getName() );
		return xppq.parse( data  );
	}
}
