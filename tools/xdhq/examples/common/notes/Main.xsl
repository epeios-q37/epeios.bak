<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" 
	xmlns="http://www.w3.org/1999/xhtml" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/XDHTML">
		<div class="menu">
			<a>
				<h1>
					<em>Notes</em>
				</h1>
			</a>
		</div>
		<div class="jumbo">
			<a class="right q37" href="http://q37.info/"/>
			<h2>Welcome!</h2>
			<p>Take some notes with <i>UnJS</i>!</p>
			<a href="http://q37.info/UnJS/" class="button">
				<xsl:text>    About   </xsl:text>
			</a>
		</div>
		<div>
			<span class="list-filter">
				<input id="Pattern" data-xdh-cast="EditionCast" placeholder="Filter By Title" class="liset-filter light" type="text" data-xdh-onevent="Search"/>
			</span>
			<div style="margin: auto; display: flex; justify-content: space-around; width: 50%;">
				<span>
					<input type="checkbox" data-xdh-cast="EditionCast" data-xdh-onevent="ToggleDescriptions"/>
					<label> Hide descriptions </label>
				</span>
				<button data-xdh-value="0" data-xdh-onevent="Edit" data-xdh-cast="EditionCast">Create</button>
			</div>
		</div>
		<div id="Notes"/>
	</xsl:template>
</xsl:stylesheet>