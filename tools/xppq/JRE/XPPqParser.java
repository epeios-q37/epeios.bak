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
