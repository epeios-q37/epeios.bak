<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet version="1.0"	xmlns="http://www.w3.org/1999/xhtml"	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/XDHTML">
		<xsl:apply-templates select="Todos"/>
	</xsl:template>
	<xsl:template match="Todos">
		<xsl:apply-templates select="Todo"/>
	</xsl:template>
	<xsl:template match="Todo">
		<li class="view">
			<span>
				<input class="toggle" type="checkbox"/>
				<label>
				 <xsl:value-of select="."/>
				</label>
				<button data-xdh-content="{@id}" class="destroy" data-xdh-onevent="Destroy"/>
			</span>
		</li>
	</xsl:template>
</xsl:stylesheet>