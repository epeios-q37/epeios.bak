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

	private int itemsLeft() {
		int count = 0;
		Iterator<Todo> it = todos.iterator();

		while (it.hasNext()) {
			if (!it.next().completed)
				count++;
		}

		return count;
	}

	private void push(Todo todo, int id, Tree tree) {
		tree.pushTag("Todo");
		tree.putAttribute("id", id);
		tree.putAttribute("completed", todo.completed);
		tree.putValue(todo.label);
		tree.popTag();
	}

	private void displayCount(DOM dom, int count) {
		String text = "";

		switch (count) {
		case 0:
			break;
		case 1:
			text = "1 item left";
			break;
		default:
			text = count + " items left";
			break;
		}

		dom.setContent("Count", text);
	}

	private void handleCount(DOM dom) {
		int count = itemsLeft();

		if (count != todos.size())
			dom.disableElement("HideClearCompleted");
		else
			dom.enableElement("HideClearCompleted");

		displayCount(dom, count);
	}

	private void displayTodos(DOM dom) {
		Tree tree = new Tree();
		ListIterator<Todo> li = todos.listIterator();

		tree.pushTag("Todos");

		while (li.hasNext()) {
			int index = li.nextIndex();
			Todo todo = li.next();

			if ((exclude == null) || (todo.completed != exclude))
				push(todo, index, tree);
		}

		tree.popTag();

		dom.setLayout("Todos", tree, "Todos.4.xsl");
		handleCount(dom);
	}

	private void submitNew(DOM dom) {
		String content = dom.getContent("Input");
		dom.setContent("Input", "");

		if (!"".equals(content.trim())) {
			todos.add(0, new Todo(content));
			displayTodos(dom);
		}
	}

	private void toggle(DOM dom, String id) {
		int index = Integer.parseInt(id);
		Todo todo = todos.get(index);

		todo.completed = !todo.completed;

		todos.set(index, todo);

		dom.toggleClass("Todo." + id, "completed");
		handleCount(dom);
	}

	private void all(DOM dom) {
		exclude = null;

		dom.addClass("All", "selected");
		dom.removeClasses(new String[][] { { "Active", "selected" }, { "Completed", "selected" } });

		displayTodos(dom);
	}

	private void active(DOM dom) {
		exclude = true;

		dom.addClass("Active", "selected");
		dom.removeClasses(new String[][] { { "All", "selected" }, { "Completed", "selected" } });

		displayTodos(dom);
	}

	private void completed(DOM dom) {
		exclude = false;

		dom.addClass("Completed", "selected");
		dom.removeClasses(new String[][] { { "All", "selected" }, { "Active", "selected" } });

		displayTodos(dom);
	}

	public void handle(DOM dom, String action, String id) {
		if (action.equals("Connect")) {
			dom.setLayout("", new Tree(), "Main.7.xsl");
		} else if (action.equals("Submit")) {
			submitNew(dom);
		} else if (action.equals("Destroy")) {
			todos.remove(Integer.parseInt(dom.getContent(id)));
			displayTodos(dom);
		} else if (action.equals("Toggle")) {
			toggle(dom, id);
		} else if (action.equals("All")) {
			all(dom);
		} else if (action.equals("Active")) {
			active(dom);
		} else if (action.equals("Completed")) {
			completed(dom);
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