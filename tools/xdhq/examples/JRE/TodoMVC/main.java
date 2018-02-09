
/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of UnJSq.

	UnJSq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	UnJSq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with UnJSq If not, see <http://www.gnu.org/licenses/>.
*/

import info.q37.unjsq.*;
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

class Thread extends java.lang.Thread {
	private DOM dom;
	private Boolean exclude;
	private int index = -1;
	private List<Todo> todos;

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

	private void displayCount(int count) {
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

	private void handleCount() {
		int count = itemsLeft();

		if (count != todos.size())
			dom.disableElement("HideClearCompleted");
		else
			dom.enableElement("HideClearCompleted");

		displayCount(count);
	}

	private void displayTodos() {
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

		dom.setLayout("Todos", tree, "Todos.xsl");
		handleCount();
	}

	private void submitNew() {
		String content = dom.getContent("Input");
		dom.setContent("Input", "");

		if (!"".equals(content.trim())) {
			todos.add(0, new Todo(content));
			displayTodos();
		}
	}

	private void submitModification() {
		int index = this.index;
		this.index = -1;

		String content = dom.getContent("Input." + index);
		dom.setContent("Input." + index, "");

		if (!"".equals(content.trim())) {
			todos.set(index, new Todo(content, todos.get(index).completed));

			dom.setContent("Label." + index, content);

			String idsAndClasses[][] = { { "View." + index, "hide" }, { "Todo." + index, "editing" } };

			dom.removeClasses(idsAndClasses);
		} else {
			todos.remove(index);
			displayTodos();
		}
	}

	private void toggle(String id) {
		int index = Integer.parseInt(id);
		Todo todo = todos.get(index);

		todo.completed = !todo.completed;

		todos.set(index, todo);

		dom.toggleClass("Todo." + id, "completed");

		if (exclude != null)
			displayTodos();
		else
			handleCount();
	}

	private void all() {
		String idsAndClasses[][] = { { "Active", "selected" }, { "Completed", "selected" } };

		exclude = null;

		dom.addClass("All", "selected");
		dom.removeClasses(idsAndClasses);

		displayTodos();
	}

	private void active() {
		String idsAndClasses[][] = { { "All", "selected" }, { "Completed", "selected" } };

		exclude = true;

		dom.addClass("Active", "selected");
		dom.removeClasses(idsAndClasses);

		displayTodos();
	}

	private void completed() {
		String idsAndClasses[][] = { { "All", "selected" }, { "Active", "selected" } };

		exclude = false;

		dom.addClass("Completed", "selected");
		dom.removeClasses(idsAndClasses);

		displayTodos();
	}

	private void clear() {
		ListIterator<Todo> li = todos.listIterator();

		while (li.hasNext()) {
			if (li.next().completed)
				li.remove();
		}

		displayTodos();
	}

	private void edit(String id) {
		String content = dom.getContent(id);
		String idsAndClasses[][] = { { "View." + content, "hide" }, { id, "editing" } };

		index = Integer.parseInt(content);

		dom.addClasses(idsAndClasses);
		dom.setContent("Input." + content, todos.get(index).label);
		dom.focus("Input." + content);
	}

	private void cancel() {
		int index = this.index;
		String idsAndClasses[][] = { { "View." + index, "hide" }, { "Todo." + index, "editing" } };

		this.index = -1;

		dom.setContent("Input." + index, "");
		dom.removeClasses(idsAndClasses);
	}

	public Thread(DOM dom) {
		this.dom = dom;
		todos = new ArrayList<Todo>();

		todos.add(new Todo("Todo 1", true));
		todos.add(new Todo("Todo 2"));
	}

	public void run() {
		Event event = new Event();

		System.out.println("Connection detected...");

		for (;;) {
			switch (dom.getAction(event)) {
			case "Connect":
				dom.setLayout("", new Tree(), "Main.xsl");
				dom.focus("Input");
				displayTodos();
				break;
			case "Submit":
				if (index == -1)
					submitNew();
				else
					submitModification();
				break;
			case "Destroy":
				todos.remove(Integer.parseInt(dom.getContent(event.id)));
				displayTodos();
				break;
			case "Toggle":
				toggle(event.id);
				break;
			case "All":
				all();
				break;
			case "Active":
				active();
				break;
			case "Completed":
				completed();
				break;
			case "Clear":
				clear();
				break;
			case "Edit":
				edit(event.id);
				break;
			case "Cancel":
				cancel();
				break;
			default:
				System.out.println("No or unknown action !");
				System.exit(1);
				break;
			}
		}
	}
}

class TodoMVC {
	public static void main(String[] args) throws Exception {
		UnJSq.listen( "TodoMVC" );

		for (;;) {
			java.lang.Thread thread = new Thread(new DOM());
			thread.start();
		}
	}
}