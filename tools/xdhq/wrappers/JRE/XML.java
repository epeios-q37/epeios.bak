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

public class XML {
	private String xml_;
	private void write_( String string ) {
		xml_ += string + '\0';
	}

	public XML( String rootTag ) {
		write_( "dummy" );
		write_( rootTag );
	}

	public void pushTag( String tag ) {
		xml_ += ">";
		write_( tag );
	}

	public void popTag() {
		xml_ += "<";
	}

	public <Value> void setAttribute( String name, Value value ) {
		xml_ += "A";
		write_( name );
		write_( value.toString() );
	}

	public <Value> void setValue( Value value ) {
		xml_ += "V";
		write_( value.toString() );
	}

	public String toString() {
		return xml_;
	}

}