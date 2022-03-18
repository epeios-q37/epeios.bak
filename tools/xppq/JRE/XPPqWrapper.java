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

import info.q37.jreq.JRE;

public class XPPqWrapper extends JRE {
	private static long launcher;
	static {
		launcher = JRE.register( "xppq" );
	}
	protected static Object call(int index, Object... objects) {
		return JRE.call( launcher, index, objects );
	}
	public static String componentInfo(){
		return JRE.componentInfo( launcher );
	}
	public static String returnArgument( String argument )
	{
		return (String)call( 0, argument );
	}
	public static void listen() {
		call( 7, "53752");
	}
}
