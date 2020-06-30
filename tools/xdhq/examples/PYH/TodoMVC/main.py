"""
MIT License

Copyright (c) 2018 Claude SIMON (https://q37.info/s/rmnmqd49)

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
"""

import os, sys

sys.path.append("./atlastk")
sys.path.append("../atlastk")

import atlastk as Atlas

def read_asset(path):
	return Atlas.read_asset(path, "TodoMVC")

class TodoMVC:
	def __init__(self):
		self.exclude = None
		self.index = -1
		self.todos = []

		if False:	# Set to 'True' for testing purpose.
			self.todos.append({"label": "Todo 1", "completed": False })
			self.todos.append({"label": "Todo 2", "completed": True })

	def items_left(self):
		count = 0

		for index in range(len(self.todos)):
			if not self.todos[index]['completed']:
				count += 1

		return count

	def push(self, todo, id, xml):
		xml.push_tag("Todo")
		xml.put_attribute("id", id)
		xml.put_attribute("completed", "true" if todo['completed'] else "false")
		xml.putValue(todo['label'])
		xml.pop_tag()

	def display_count(self, dom, count):
		text = ""

		if count == 1:
			text = "1 item left"
		elif count != 0:
			text = str(count) + " items left"

		dom.set_content("Count", text)

	def handle_count(self, dom):
		count = self.items_left()

		if count != len(self.todos):
			dom.disable_element("HideClearCompleted")
		else:
			dom.enable_element("HideClearCompleted")

		self.display_count(dom, count)

	def display_todos(self, dom):
		xml = Atlas.create_XML("XDHTML")

		xml.push_tag("Todos")

		for index in range(len(self.todos)):
			todo = self.todos[index]

			if (self.exclude == None) or (todo['completed'] != self.exclude):
				self.push(todo, index, xml)

		xml.pop_tag()

		dom.set_layout_XSL("Todos", xml, "Todos.xsl")
		self.handle_count(dom)

	def submit_new(self, dom):
		content = dom.get_content("Input").strip()
		dom.set_content("Input", "")

		if content:
			self.todos.insert(0, {'label': content, 'completed': False})
			self.display_todos(dom)

	def submit_modification(self, dom):
		index = self.index
		self.index = -1

		content = dom.get_content("Input." + str(index)).strip()
		dom.set_content("Input." + str(index), "")

		if content:
			self.todos[index]['label'] = content

			dom.set_content("Label." + str(index), content)

			dom.remove_classes({"View." + str(index): "hide", "Todo." + str(index): "editing"})
		else:
			self.todos.pop(index)
			self.displayTodos(dom)

def ac_connect(self, dom):
	dom.set_layout("", read_asset("Main.html"))
	dom.focus("Input")
	self.display_todos(dom)
	dom.disable_elements(["HideActive", "HideCompleted"])

def ac_destroy(self, dom, id):
	self.todos.pop(int(dom.get_content(id)))
	self.display_todos(dom)

def ac_toggle(self, dom, id):
	index = int(id)
	self.todos[index]['completed'] = not self.todos[index]['completed']

	dom.toggle_class("Todo." + id, "completed")
	dom.toggle_class("Todo." + id, "active")

	self.handle_count(dom)

def ac_all(self, dom):
	self.exclude = None

	dom.add_class("All", "selected")
	dom.remove_classes({"Active": "selected", "Completed": "selected"})
	dom.disable_elements(["HideActive", "HideCompleted"])

def ac_active(self, dom):
	self.exclude = True

	dom.add_class("Active", "selected")
	dom.remove_classes({"All": "selected", "Completed": "selected"})
	dom.disable_element("HideActive")
	dom.enable_element("HideCompleted")

def ac_completed(self, dom):
	self.exclude = False

	dom.add_class("Completed", "selected")
	dom.remove_classes({"All": "selected", "Active": "selected"})
	dom.disable_element("HideCompleted")
	dom.enable_element("HideActive")

def ac_clear(self, dom):
	index = len(self.todos)

	while index:
		index -= 1

		if self.todos[index]['completed']:
			self.todos.pop(index)

	self.display_todos(dom)

def ac_edit(self, dom, id):
	content = dom.get_content(id)
	self.index = int(content)

	dom.add_classes({"View." + content: "hide", id: "editing"})
	dom.set_content("Input." + content, self.todos[self.index]['label'])
	dom.focus("Input." + content)

def ac_cancel(self, dom):
	index = str(self.index)
	self.index = -1

	dom.set_content("Input." + index, "")
	dom.remove_classes({"View." + index: "hide", "Todo." + index: "editing"})

callbacks = {
	"": ac_connect,
	"Submit": lambda self, dom: self.submit_new(dom) if self.index == -1 else self.submit_modification(dom),
	"Destroy": ac_destroy,
	"Toggle": ac_toggle,
	"All": ac_all,
	"Active": ac_active,
	"Completed": ac_completed,
	"Clear": ac_clear,
	"Edit": ac_edit,
	"Cancel": ac_cancel,
}

Atlas.launch(callbacks, TodoMVC, read_asset("HeadFaaS.html"), "TodoMVC")
