﻿<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				>
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Context"/>
	</xsl:template>
	<xsl:template match="Context">
		<xsl:choose>
			<xsl:when test="Dragging='InProgress'">
				<xdh-cast id="EntryDraggingCast" kind="Undraggable"/>
				<xdh-cast id="EntryDroppingCast" kind="Droppable"/>
			</xsl:when>
			<xsl:otherwise>
				<xdh-cast id="EntryDraggingCast" kind="Draggable"/>
				<xdh-cast id="EntryDroppingCast" kind="Undroppable"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>