<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" 
	xmlns="http://www.w3.org/1999/xhtml" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
	xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<section class="todoapp">
			<header class="header">
				<h1>todos</h1>
				<input class="new-todo" placeholder="What needs to be done?" autofocus="" data-xdh-onevent="Submit"/>
			</header>
			<section class="main">
				<input class="toggle-all" type="checkbox"/>
					<label for="toggle-all">Mark all as complete</label>
					<ul class="todo-list" id="Todos"/>
				</section>
			<footer class="footer">
				<span class="todo-count" id="Count"></span>
				<ul class="filters">
					<li>
						<a href="#/" class="selected">All</a>
					</li>
					<li>
						<a href="#/active">Active</a>
					</li>
					<li>
						<a href="#/completed">Completed</a>
					</li>
				</ul>
				<button class="clear-completed">Clear completed</button>
			</footer>
		</section>
		<footer class="info">
			<p>Double-click to edit a todo</p>
			<p>
				Created by <a href="http://q37.info/contact/">Claude SIMON</a>
			</p>
			<!--p>
				Part of <a href="http://todomvc.com">TodoMVC</a>
			</p-->
		</footer>	</xsl:template>
</xsl:stylesheet>