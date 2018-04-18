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
	along with XDHq If not, see <http://www.gnu.org/licenses/>.
*/

import info.q37.atlas.*;

class Thread extends java.lang.Thread {
	private DOM dom;

	public Thread(DOM dom) {
		this.dom = dom;
	}

	public void run() {
		Event event = new Event();
		for (;;) {
			if ( false ) {
				String action = dom.getAction(event);

				if ( action.equals( "Connect" ) ) {
					dom.setLayout("", new Tree(), "Main.xsl");
				} else if ( action.equals( "Typing" ) ) {
					dom.setContent("name", dom.getContent(event.id));
				}
			} else {
				switch( dom.getAction( event )) {
				case "Connect":
					dom.setLayout("", new Tree(), "Main.xsl");
					break;
				case "Typing":
					dom.setContent("name", dom.getContent(event.id));
					break;
				}
			}
		}
	}
}

class Hello {
	public static void main(String[] args) throws Exception {
		Atlas.launch("Connect", "Hello", Atlas.Type.DEFAULT, args );

		for (;;) (new Thread(new DOM())).start();
	}
}