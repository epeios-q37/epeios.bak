/*
	Copyright (C) 2018 Claude SIMON (http://q37.info/contact/).

	This file is part of XDHq.

	XDHq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	XDHq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with XDHq. If not, see <http://www.gnu.org/licenses/>.
*/

package info.q37.xdhq;

import info.q37.xdhq.dom.DOM_SHRD.Type;

public class ARG {
	public Type type;
	public String string;
	public String[] strings;

	public ARG( String string ) {
		this.type = Type.STRING;
		this.string = string;
	}

	public ARG( String[] strings ) {
		this.type = Type.STRINGS;
		this.strings = strings;
	}
}