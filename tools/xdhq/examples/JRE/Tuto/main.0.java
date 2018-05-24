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
import java.util.*;

class TodoMVC extends Atlas {
	public void handle(DOM dom, String action, String id) {
		if (action.equals("Connect")) {
			dom.setLayout("", new Tree(), "Main.0.xsl");
		} else {
			System.out.println("No or unknown action !");
			System.exit(1);
		}
	}

	public static void main(String[] args) throws Exception {
		String dir;

		if (System.getenv("EPEIOS_SRC") == null)
			dir = ".";
		else
			dir = "Tuto";

		launch("Connect", dir, Atlas.Type.DEFAULT, args);

		for (;;)
			new TodoMVC();
	}
}