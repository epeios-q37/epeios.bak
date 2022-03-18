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

import info.q37.jreq.Core;

public class XPPq extends XPPqWrapper {
	public Core core;
	public XPPq(long pointer)
	{
		core = new Core(pointer);
	}
	
	public static long getParser( java.io.InputStream stream )
	{
		return ( (java.lang.Long)call( 11, stream )).longValue();
	}
	
	public int parse( XPPqData data )
	{
		return ( (java.lang.Integer)call( 12, core, data ) ).intValue();
	}
	
	public static long getPreprocessor( java.io.InputStream stream )
	{
		return ( (java.lang.Long)call( 21, stream )).longValue();
	}
	
	public int readFromPreprocessor()
	{
		return ( (java.lang.Integer)call( 22, core ) ).intValue();
	}

	public int readFromPreprocessor(byte[] b, int off, int len) 
	{
		// Not implemented yet (see C++ source) !
		return ( (java.lang.Integer)call( 23, core, b, off, len ) ).intValue();		
	}	
}

