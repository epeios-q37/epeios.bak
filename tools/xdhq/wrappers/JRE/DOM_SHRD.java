/*
	Copyright (C) 2017 Claude SIMON (http://q37.info/contact/).

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

package info.q37.xdhq.dom;

public abstract class DOM_SHRD {
	public enum Type {
		VOID( 0 ), STRING( 1 ), STRINGS( 2 ), UNDEFINED( 3 );
	    private int value;
	    private Type(int value) {
			this.value = value;
		}
	    public int getValue() {
			return value;
		}
	};

	public abstract void getAction(Event event);

	public abstract Object call(String command, Type type, String[] strings, String [][] xstrings );
}
