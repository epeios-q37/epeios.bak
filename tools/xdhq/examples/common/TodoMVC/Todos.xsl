<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/XDHTML">
		<xsl:apply-templates select="Todos"/>
	</xsl:template>
	<xsl:template match="Todos">
			<xsl:apply-templates select="Todo"/>
	</xsl:template>
	<xsl:template match="Todo">
		<li class="view" id="Todo.{@id}" data-xdh-onevents="(dblclick|Edit)" data-xdh-value="{@id}">
			<xsl:choose>
			<xsl:when test="@completed='true'">
				<xsl:attribute name="class">completed</xsl:attribute>
			</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="class">active</xsl:attribute>
				</xsl:otherwise>
			</xsl:choose>
			<span id="View.{@id}">
				<input class="toggle" type="checkbox" id="{@id}" data-xdh-onevent="Toggle">
					<xsl:if test="@completed='true'">
						<xsl:attribute name="checked"></xsl:attribute>
					</xsl:if>
				</input>
				<label id="Label.{@id}">
					<xsl:value-of select="."/>
				</label>
				<button data-xdh-value="{@id}" class="destroy" data-xdh-onevent="Destroy"/>
			</span>
				<input id="Input.{@id}" class="edit" data-xdh-onevents="(keypress|Cancel|Esc)(keypress|Submit|Enter)(blur|Submit)"/>
		</li>
	</xsl:template>
</xsl:stylesheet>