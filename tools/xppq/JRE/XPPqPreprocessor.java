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

public class XPPqPreprocessor extends java.io.InputStream {
	private XPPq xppq;
	public XPPqPreprocessor( java.io.InputStream Stream )
	{
		xppq = new XPPq( XPPq.getPreprocessor( Stream ) );
	}
	
 	@Override
	public int read()
	{
		return xppq.readFromPreprocessor();
	}
	/*
		Commented so that a call of below function will call above one,
		as the native corresponding function in not implemented yet.
	*/
	/*
  @Override
  public int read(byte[] b, int off, int len) {
		return xppq.readFromPreprocessor(b, off, len);
  }
*/	
}
