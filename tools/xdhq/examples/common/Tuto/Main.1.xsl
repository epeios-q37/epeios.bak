<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	xmlns="http://www.w3.org/1999/xhtml"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xdh="http://q37.info/ns/xdh">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<section class="todoapp">
			<header class="header">
				<h1>todos</h1>
				<input id="Input" class="new-todo" placeholder="What needs to be done?" autofocus="" xdh:onevent="keypress|Submit|Enter"/>
			</header>
			<section class="main">
				<input class="toggle-all" type="checkbox"/>
				<label for="toggle-all">Mark all as complete</label>
				<ul class="todo-list" id="Todos"/>
			</section>
			<footer class="footer">
				<span class="todo-count" id="Count"/>
				<ul class="filters">
					<li>
						<a style="cursor: pointer;" id="All" class="selected">All</a>
					</li>
					<li>
						<a style="cursor: pointer;" id="Active">Active</a>
					</li>
					<li>
						<a style="cursor: pointer;" id="Completed">Completed</a>
					</li>
				</ul>
				<button class="clear-completed">Clear completed</button>
			</footer>
		</section>
		<footer class="info">
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
	</xsl:template>
</xsl:stylesheet>