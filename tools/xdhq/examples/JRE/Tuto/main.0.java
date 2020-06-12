/*
MIT License

Copyright (c) 2017 Claude SIMON (https://q37.info/s/rmnmqd49)

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

import info.q37.atlas.*;
import java.util.*;

class Todo {
	public boolean completed;
	public String label;

	Todo(String label, boolean completed) {
		this.completed = completed;
		this.label = label;
	}

	Todo(String label) {
		this(label, false);
	}
}

class TodoMVC extends Atlas {
	private Boolean exclude;
	private int index = -1;
	private List<Todo> todos;

	public TodoMVC() {
		todos = new ArrayList<Todo>();
	}

	public void handle(DOM dom, String action, String id) {
		if (action.equals("Connect")) {
			dom.setLayout("", new Tree(), "Main.0.xsl");
		} else {
			System.out.println("No or unknown action !");
			System.exit(1);
		}
	}

	public static void main(String[] args) {
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