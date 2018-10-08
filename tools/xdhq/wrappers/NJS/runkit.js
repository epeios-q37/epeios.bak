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

const atlas = require("atlastk@0.3.0");
const DOM = atlas.DOM;

class MyData extends DOM {
	constructor() {
		super();
		this.timestamp = new Date();
		this.exclude = null;
		this.index = -1;
		this.todos = [
			{
				"completed": true,
				"label": "Note 1"
			}, {
				"completed": false,
				"label": "Note 2"
			}
		];
		this.todos = [];
	}
	itemsLeft() {
		var i = this.todos.length;
		var count = 0;

		while (i--) {
			if (!this.todos[i]['completed'])
				count++;
		}

		return count;
	}
}

function displayCount(dom, count) {
	var text = "";

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

function handleCount(dom) {
	var count = dom.itemsLeft();

	if (count != dom.todos.length)
		dom.disableElement("HideClearCompleted",
			() => displayCount(dom, count)
		)
	else
		dom.enableElement("HideClearCompleted",
			() => displayCount(dom, count)
		)
}

function displayTodos(dom) {
	var tree = require('xmlbuilder').create('XDHTML');
	var i = 0;
	var todo;

	dom.index = -1;

	tree = tree.ele("Todos");

	while (i < dom.todos.length) {
		todo = dom.todos[i];
		tree = tree.ele('Todo', { 'id': i, 'completed': todo["completed"] }, todo["label"]).up();
		i++;
	}

	tree = tree.up();

	dom.setLayout_("Todos", tree.end(), "data:text/xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiPz4NCjwhLS0gTk8gQk9NICEhIC0tPg0KPHhzbDpzdHlsZXNoZWV0IHZlcnNpb249IjEuMCIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkveGh0bWwiIHhtbG5zOnhzbD0iaHR0cDovL3d3dy53My5vcmcvMTk5OS9YU0wvVHJhbnNmb3JtIj4NCgk8eHNsOm91dHB1dCBtZXRob2Q9Imh0bWwiIGVuY29kaW5nPSJVVEYtOCIvPg0KCTx4c2w6dGVtcGxhdGUgbWF0Y2g9Ii9YREhUTUwiPg0KCQk8eHNsOmFwcGx5LXRlbXBsYXRlcyBzZWxlY3Q9IlRvZG9zIi8+DQoJPC94c2w6dGVtcGxhdGU+DQoJPHhzbDp0ZW1wbGF0ZSBtYXRjaD0iVG9kb3MiPg0KCQk8eHNsOmFwcGx5LXRlbXBsYXRlcyBzZWxlY3Q9IlRvZG8iLz4NCgk8L3hzbDp0ZW1wbGF0ZT4NCgk8eHNsOnRlbXBsYXRlIG1hdGNoPSJUb2RvIj4NCgkJPGxpIGlkPSJUb2RvLntAaWR9IiBkYXRhLXhkaC1vbmV2ZW50cz0iKGRibGNsaWNrfEVkaXQpIiBkYXRhLXhkaC12YWx1ZT0ie0BpZH0iPg0KCQkJPHhzbDphdHRyaWJ1dGUgbmFtZT0iY2xhc3MiPg0KCQkJCTx4c2w6dGV4dD52aWV3PC94c2w6dGV4dD4NCgkJCQk8eHNsOmNob29zZT4NCgkJCQkJPHhzbDp3aGVuIHRlc3Q9IkBjb21wbGV0ZWQ9J3RydWUnIj4NCgkJCQkJCTx4c2w6dGV4dD4gY29tcGxldGVkPC94c2w6dGV4dD4NCgkJCQkJPC94c2w6d2hlbj4NCgkJCQkJPHhzbDpvdGhlcndpc2U+DQoJCQkJCQk8eHNsOnRleHQ+IGFjdGl2ZTwveHNsOnRleHQ+DQoJCQkJCTwveHNsOm90aGVyd2lzZT4NCgkJCQk8L3hzbDpjaG9vc2U+DQoJCQk8L3hzbDphdHRyaWJ1dGU+DQoJCQk8c3BhbiBpZD0iVmlldy57QGlkfSI+DQoJCQkJPGlucHV0IGNsYXNzPSJ0b2dnbGUiIHR5cGU9ImNoZWNrYm94IiBpZD0ie0BpZH0iIGRhdGEteGRoLW9uZXZlbnQ9IlRvZ2dsZSI+DQoJCQkJCTx4c2w6aWYgdGVzdD0iQGNvbXBsZXRlZD0ndHJ1ZSciPg0KCQkJCQkJPHhzbDphdHRyaWJ1dGUgbmFtZT0iY2hlY2tlZCIvPg0KCQkJCQk8L3hzbDppZj4NCgkJCQk8L2lucHV0Pg0KCQkJCTxsYWJlbCAgaWQ9IkxhYmVsLntAaWR9Ij4NCgkJCQkJPHhzbDp2YWx1ZS1vZiBzZWxlY3Q9Ii4iLz4NCgkJCQk8L2xhYmVsPg0KCQkJCTxidXR0b24gZGF0YS14ZGgtdmFsdWU9IntAaWR9IiBjbGFzcz0iZGVzdHJveSIgZGF0YS14ZGgtb25ldmVudD0iRGVzdHJveSIvPg0KCQkJPC9zcGFuPg0KCQkJPGlucHV0IGlkPSJJbnB1dC57QGlkfSIgY2xhc3M9ImVkaXQiIGRhdGEteGRoLW9uZXZlbnRzPSIoa2V5dXB8Q2FuY2VsfEVzYykoa2V5cHJlc3N8U3VibWl0fEVudGVyKShibHVyfFN1Ym1pdCkiLz4NCgkJPC9saT4NCgk8L3hzbDp0ZW1wbGF0ZT4NCjwveHNsOnN0eWxlc2hlZXQ+",
		() => handleCount(dom)
	);
}

function newSession() {
	return new MyData();
}

function acConnect(dom, id) {
	dom.setLayout("", body,
		() => dom.focus("Input",
			() => dom.disableElements(["HideActive", "HideCompleted"],
				() => displayTodos(dom)
			)
		)
	);
}

function submitNew(dom) {
	dom.getContent("Input",
		(content) => dom.setContent("Input", "",
			() => {
				if (content.trim() != "") {
					dom.todos.unshift(
						{
							"completed": false,
							"label": content
						}
					);
					displayTodos(dom);
				}
			}
		)
	);
}

function submitModification(dom) {
	let id = dom.index;
	dom.index = -1;

	dom.getContent("Input." + id,
		(content) => dom.setContent("Input." + id, "",
			() => {
				if (content.trim() != "") {
					dom.todos[id]['label'] = content;
					dom.setContent("Label." + id, content,
						() => dom.removeClasses(
							{
								["View." +id]: "hide",
								["Todo." +id]: "editing"
							}
						)
					);
				} else {
					dom.todos.splice(id, 1);
					displayTodos(dom);
				}
			}
		)
	);
}

function acSubmit(dom, id) {
	if (dom.index == -1) {
		submitNew(dom);
	} else {
		submitModification(dom);
	}
}

function acDestroy(dom, id) {
	dom.getContent(id,
		(content) => {
			dom.todos.splice(parseInt(content), 1);
			displayTodos(dom);
		}
	);
}

function acToggle(dom, id) {
	var i = parseInt(id);

	dom.todos[i]['completed'] = !dom.todos[i]['completed'];

	// Can't use 'ToggleClasses', because then 2 elements would have same key...
	dom.toggleClass("Todo." + id, "completed",
		() => dom.toggleClass("Todo." + id, "active",
			() => handleCount(dom)
		)
	);
}

function acAll(dom, id) {
	dom.exclude = null;

	dom.addClass("All", "selected",
		() => dom.removeClasses(
			{
				"Active": "selected",
				"Completed": "selected"
			},
			() => dom.disableElements(["HideActive", "HideCompleted"])
		)
	)
}

function acActive(dom, id) {
	dom.exclude = true;

	dom.addClass("Active", "selected",
		() => dom.removeClasses(
			{
				"All": "selected",
				"Completed": "selected"
			},
			() => dom.disableElement("HideActive",
				() => dom.enableElement("HideCompleted"))
		)
	)
}

function acCompleted(dom, id) {
	dom.exclude = false;

	dom.addClass("Completed", "selected",
		() => dom.removeClasses(
			{
				"All": "selected",
				"Active": "selected"
			},
			() => dom.disableElement("HideCompleted",
				() =>dom.enableElement("HideActive"))
		)
	)
}

function acClear(dom, id) {
	var i = 0;

	while (i < dom.todos.length) {
		if (dom.todos[i]['completed'])
			dom.todos.splice(i, 1);
		else
			i++;
	}

	displayTodos(dom);
}

function acEdit(dom, id) {
	dom.getContent(id,
		(content) => dom.addClasses(
			{
				["View." +content]: "hide",
				[id]: "editing"
			},
			() => {
				dom.index = parseInt(content);
				dom.setContent("Input." + content, dom.todos[dom.index]['label'],
					() => dom.focus("Input." + content))
			}
		)
	);
}

function acCancel(dom, id) {
	var id = dom.index;
	dom.index = -1;

	dom.setContent("Input." + id, "",
		() => dom.removeClasses(
			{
				["View." +id]: "hide",
				["Todo." +id]: "editing"
			}
		)
	);
}

function main() {
	var callbacks = {
		"Connect": acConnect,
		"Submit": acSubmit,
		"Destroy": acDestroy,
		"Toggle": acToggle,
		"All": acAll,
		"Active": acActive,
		"Completed": acCompleted,
		"Clear": acClear,
		"Edit": acEdit,
		"Cancel": acCancel,
	};

	// Content of 'HeadDEMO.html'.
	let head = `
		<title>Atlas • TodoMVC</title>
		<link rel="icon" type="image/png" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgBAMAAACBVGfHAAAAMFBMVEUEAvyEhsxERuS8urQsKuycnsRkYtzc2qwUFvRUVtysrrx0ctTs6qTMyrSUksQ0NuyciPBdAAABHklEQVR42mNgwAa8zlxjDd2A4POfOXPmzZkFCAH2M8fNzyALzDlzg2ENssCbMwkMOsgCa858YOjBKxBzRoHhD7LAHiBH5swCT9HQ6A9ggZ4zp7YCrV0DdM6pBpAAG5Blc2aBDZA68wCsZPuZU0BDH07xvHOmAGKKvgMP2NA/Zw7ADIYJXGDgLQeBBSCBFu0aoAPYQUadMQAJAE29zwAVWMCWpgB08ZnDQGsbGhpsgCqBQHNfzRkDEIPlzFmo0T5nzoMovjPHoAK8Zw5BnA5yDosDSAVYQOYMKIDZzkoDzagAsjhqzjRAfXTmzAQgi/vMQZA6pjtAvhEk0E+ATWRRm6YBZuScCUCNN5szH1D4TGdOoSrggtiNAH3vBBjwAQCglIrSZkf1MQAAAABJRU5ErkJggg==" />
		<!-- Only both lines below change between 'PROD' and 'DEMO' files. -->
		<link rel="stylesheet" href="http://q37.info/download/assets/TodoMVC/todomvc-common/base.css">
		<link rel="stylesheet" href="http://q37.info/download/assets/TodoMVC/todomvc-app-css/index.css">
		<style>
		 .hide {
				display: none;
			}
		</style>
		<style id="HideClearCompleted">
		 .clear-completed {
				display: none;
			}
		</style>
		<style id="HideCompleted">
		 .completed {
				display: none;
			}
		</style>
		<style id="HideActive">
		 .active {
				display: none;
			}
		</style>
	`;

	atlas.launch(newSession, "Connect", callbacks, head);

	// Content of 'Main.html'.
	body = `
		<section class ="todoapp">
			<header class ="header">
				<h1>todos</h1>
				<input id="Input" class ="new-todo" placeholder="What needs to be done?" autofocus="" data-xdh-onevent="keypress|Submit|Enter"/>
			</header>
			<section class ="main">
				<input class ="toggle-all" type="checkbox"/>
				<label for="toggle-all">Mark all as complete</label>
				<ul class ="todo-list" id="Todos"/>
			</section>
			<footer class ="footer">
				<span class ="todo-count" id="Count"></span>
				<ul class ="filters">
					<li>
						<a style="cursor: pointer;" id="All" class ="selected" data-xdh-onevent="All">All</a>
					</li>
					<li>
						<a style="cursor: pointer;" id="Active" data-xdh-onevent="Active">Active</a>
					</li>
					<li>
						<a style="cursor: pointer;" id="Completed" data-xdh-onevent="Completed">Completed</a>
					</li>
				</ul>
				<button class ="clear-completed" data-xdh-onevent="Clear">Clear completed</button>
			</footer>
		</section>
		<footer class ="info">
			<p>Double-click to edit a todo</p>
			<p>
				<span>Created with the </span>
				<a href="http://atlastk.org/">
					<span style="font-style: italic;">Atlas</span>
					<span> toolkit</span>
				</a>
				<span>!</span>
			</p>
		</footer>
	`;
};

main();

"The program will be stopped after a while due to a RunKit timeout."