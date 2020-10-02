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
		<li id="Todo.{@id}">
			<xsl:attribute name="class">
				<xsl:text>view</xsl:text>
				<xsl:if test="@completed='true'">
					<xsl:text> completed</xsl:text>
				</xsl:if>
			</xsl:attribute>
			<span>
				<input class="toggle" type="checkbox" id="{@id}" data-xdh-onevent="Toggle">
					<xsl:if test="@completed='true'">
						<xsl:attribute name="checked"/>
					</xsl:if>
				</input>
				<label>
					<xsl:value-of select="."/>
				</label>
				<button data-xdh-mark="{@id}" class="destroy" data-xdh-onevent="Destroy"/>
			</span>
		</li>
	</xsl:template>
</xsl:stylesheet>