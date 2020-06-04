
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

	static private String readAsset_( String path )  {
		return readAsset( path, "TodoMVC" );
	}

	private int itemsLeft() {
		return (int) todos.stream().filter(todo -> !todo.completed).count();
	}

	private void push(Todo todo, int id, info.q37.xdhq.XML xml) {
		xml.pushTag("Todo");
		xml.putAttribute("id", id);
		xml.putAttribute("completed", todo.completed);
		xml.putValue(todo.label);
		xml.popTag();
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
		ListIterator<Todo> li = todos.listIterator();

		info.q37.xdhq.XML xml = Atlas.createXML("XDHTML");

		xml.pushTag("Todos");

		while (li.hasNext()) {
			int index = li.nextIndex();

			Todo todo = li.next();

			if ((exclude == null) || (todo.completed != exclude))
				push(todo, index, xml);
		}

		xml.popTag();

		dom.setLayoutXSL("Todos", xml, "Todos.xsl");
		handleCount(dom);
	}

	private void submitNew(DOM dom) {
		String content = dom.getContent("Input").trim();
		dom.setContent("Input", "");

		if (!"".equals(content)) {
			todos.add(0, new Todo(content));
			displayTodos(dom);
		}
	}

	private void submitModification(DOM dom) {
		int index = this.index;
		this.index = -1;

		String content = dom.getContent("Input." + index).trim();
		dom.setContent("Input." + index, "");

		if (!"".equals(content)) {
			todos.set(index, new Todo(content, todos.get(index).completed));

			dom.setContent("Label." + index, content);

			dom.removeClasses(new HashMap<String, String>() {
				{
					put("View." + index, "hide");
					put("Todo." + index, "editing");
				}
			});
		} else {
			todos.remove(index);
			displayTodos(dom);
		}
	}

	private void toggle(DOM dom, String id) {
		int index = Integer.parseInt(id);
		Todo todo = todos.get(index);

		todo.completed = !todo.completed;

		todos.set(index, todo);

		dom.toggleClass("Todo." + id, "completed");
		dom.toggleClass("Todo." + id, "active");

		handleCount(dom);
	}

	private void all(DOM dom) {
		exclude = null;

		dom.addClass("All", "selected");
		dom.removeClasses(new HashMap<String, String>() {
			{
				put("Active", "selected");
				put("Completed", "selected");
			}
		});
		dom.disableElements(new String[] { "HideActive", "HideCompleted" });
	}

	private void active(DOM dom) {
		exclude = true;

		dom.addClass("Active", "selected");
		dom.removeClasses(new HashMap<String, String>() {
			{
				put("All", "selected");
				put("Completed", "selected");
			}
		});
		dom.disableElement("HideActive");
		dom.enableElement("HideCompleted");
	}

	private void completed(DOM dom) {
		exclude = false;

		dom.addClass("Completed", "selected");
		dom.removeClasses(new HashMap<String, String>() {
			{
				put("All", "selected");
				put("Active", "selected");
			}
		});
		dom.disableElement("HideCompleted");
		dom.enableElement("HideActive");
	}

	private void clear(DOM dom) {
		todos.removeIf(todo -> todo.completed);

		displayTodos(dom);
	}

	private void edit(DOM dom, String id) {
		String content = dom.getContent(id);

		index = Integer.parseInt(content);

		dom.addClasses(new HashMap<String, String>() {
			{
				put("View." + content, "hide");
				put(id, "editing");
			}
		});
		dom.setContent("Input." + content, todos.get(index).label);
		dom.focus("Input." + content);
	}

	private void cancel(DOM dom) {
		int index = this.index;
		this.index = -1;

		dom.setContent("Input." + index, "");
		dom.removeClasses(new HashMap<String, String>() {
			{
				put("View." + index, "hide");
				put("Todo." + index, "editing");
			}
		});
	}

	public TodoMVC() {
		todos = new ArrayList<Todo>();

		if (false) { // At true for testing purpose.
			todos.add(new Todo("Todo 1", true));
			todos.add(new Todo("Todo 2"));
		}
	}

	@Override
	public void handle(String action, String id) {
		switch (action) {
		case "":
			dom.setLayout("", readAsset_("Main.html"));
			dom.focus("Input");
			dom.disableElements(new String[] { "HideActive", "HideCompleted" });
			displayTodos(dom);
			break;
		case "Submit":
			if (index == -1)
				submitNew(dom);
			else
				submitModification(dom);
			break;
		case "Destroy":
			todos.remove(Integer.parseInt(dom.getContent(id)));
			displayTodos(dom);
			break;
		case "Toggle":
			toggle(dom, id);
			break;
		case "All":
			all(dom);
			break;
		case "Active":
			active(dom);
			break;
		case "Completed":
			completed(dom);
			break;
		case "Clear":
			clear(dom);
			break;
		case "Edit":
			edit(dom, id);
			break;
		case "Cancel":
			cancel(dom);
			break;
		default:
			throw new RuntimeException("No or unknown action !!!");
		}
	}

	public static void main(String[] args) {
		String dir;

		launch(() -> new TodoMVC(), readAsset_("HeadFaaS.html"), "TodoMVC", GUI.DEFAULT, args);
	}
}
