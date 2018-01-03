<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/XDHTML">
		<xsl:apply-templates select="Notes"/>
	</xsl:template>
	<xsl:template match="Notes">
		<ul>
			<li>
				<span id="View.0"/>
				<span id="Edit.0"/>
			</li>
			<xsl:apply-templates select="Note"/>
		</ul>
	</xsl:template>
	<xsl:template match="Note">
		<li>
			<span id="View.{@id}">
				<div>
					<article class="listing" style="display: flex; width:100%; justify-content: space-between;align-items: center;">
						<div>
							<h3 id="Title.{@id}">
								<xsl:value-of select="title"/>
							</h3>
							<p id="Description.{@id}" data-xdh-cast="DescriptionCast">
								<!-- 'disable-output-escaping' does not work with Firefox (https://bugzilla.mozilla.org/show_bug.cgi?id=98168)	-->
								<xsl:value-of select="description" disable-output-escaping="yes"/>
							</p>
						</div>
						<span style="display: flex; flex-direction: column;">
							<button data-xdh-onevent="Edit" data-xdh-value="{@id}" data-xdh-cast="EditionCast">Edit</button>
							<button data-xdh-onevent="Delete" data-xdh-value="{@id}" data-xdh-cast="EditionCast">Delete</button>
						</span>
					</article>
				</div>
			</span>
			<span id="Edit.{@id}"/>
		</li>
	</xsl:template>
</xsl:stylesheet>