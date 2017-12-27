<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/XDHTML[@Background='Layout']">
		<xsl:apply-templates select="Todos"/>
	</xsl:template>
	<xsl:template match="Todos">
		<ul>
			<xsl:apply-templates select="Todo"/>
		</ul>
	</xsl:template>
	<xsl:template match="Todo">
		<li>
			<span>
				<xsl:attribute name="id">
					<xsl:text>View.</xsl:text>
					<xsl:value-of select="@id"/>
				</xsl:attribute>
				<div class="ember-view">
					<article class="listing" style="display: flex; width:100%; justify-content: space-between;align-items: center;">
						<div>
							<h3>
								<xsl:value-of select="title"/>
							</h3>
							<p class="description" data-xdh-cast="DescriptionCast">
								<xsl:value-of select="description"/>
							</p>
						</div>
						<span style="display: flex; flex-direction: column;">
							<button data-xdh-onevent="Edit" data-xdh-value="{@id}">Edit</button>
							<button data-xdh-onevent="Delete" data-xdh-value="{@id}">Delete</button>
						</span>
					</article>
				</div>
			</span>
			<span id="Edit.{@id}"/>
			</li>
	</xsl:template>
</xsl:stylesheet>