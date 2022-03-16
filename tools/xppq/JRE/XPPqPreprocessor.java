/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of XPPq.

	XPPq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XPPq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XPPq. If not, see <http://www.gnu.org/licenses/>.
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
