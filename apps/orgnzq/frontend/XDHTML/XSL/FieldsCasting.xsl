<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Casting"/>
	</xsl:template>
	<xsl:template match="Casting">
		<xsl:choose>
			<xsl:when test="Dragging='InProgress'">
				<xdh-cast id="FieldDraggingCast" kind="Undraggable"/>
				<xdh-cast id="FieldDroppingCast" kind="Droppable"/>
			</xsl:when>
			<xsl:otherwise>
				<xdh-cast id="FieldDraggingCast" kind="Draggable"/>
				<xdh-cast id="FieldDroppingCast" kind="Undroppable"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>