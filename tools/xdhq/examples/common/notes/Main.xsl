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
			<p>
				<span>Let's take some notes with </span>
				<span style="font-style: oblique; font-weight: bold;">Atlas</span>
				<span>!</span>
			</p>
			<a href="http://atlastk.org/" class="button">
				<xsl:text>    About   </xsl:text>
			</a>
		</div>
		<div>
			<span class="list-filter">
				<input id="Pattern" placeholder="Filter By Title" class="liset-filter light" type="text" data-xdh-onevent="Search"/>
			</span>
			<div style="margin: auto; display: flex; justify-content: space-around; width: 50%;">
				<span>
					<label>
						<input id="DescriptionToggling" type="checkbox" data-xdh-onevent="ToggleDescriptions"/>
						<span> Hide descriptions </span>
					</label>
				</span>
				<button class="button" id="CreateButton" data-xdh-value="0" data-xdh-onevent="Edit">Create</button>
			</div>
		</div>
		<div id="Notes"/>
	</xsl:template>
</xsl:stylesheet>