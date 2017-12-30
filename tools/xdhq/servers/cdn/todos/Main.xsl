<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/XDHTML">
		<div class="menu">
			<a>
				<h1>
					<em>Todos</em>
				</h1>
			</a>
		</div>
		<div class="jumbo">
			<div class="right q37"/>
			<h2>Welcome!</h2>
			<p>This is an example of use of the XDHTML technology !</p>
			<a href="http://q37.info/" class="button">
				<xsl:text>    About Us   </xsl:text>
			</a>
		</div>
		<div class="list-filter">
			<div style="display: flex; width: 80%; margin: auto;">
				<input id="Pattern"  data-xdh-cast="EditionCast" placeholder="Filter By Title" class="light" type="text" data-xdh-onevent="Search"/>
				<div>
				<div style="display: flex; flex-direction: row;">
						<input type="checkbox" data-xdh-cast="EditionCast" data-xdh-onevent="ToggleDescriptions"/>
						<button data-xdh-value="0" data-xdh-onevent="Edit" data-xdh-cast="EditionCast">Create</button>
				</div>
				</div>
			</div>
			<div id="Todos"/>
		</div>
	</xsl:template>
</xsl:stylesheet>