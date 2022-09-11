<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				>
	<xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Casting"/>
	</xsl:template>
	<xsl:template match="Casting">
		<xsl:element name="xdh-cast">
			<xsl:attribute name="id">
				<xsl:text>FieldDefinition</xsl:text>
			</xsl:attribute>
			<xsl:attribute name="kind">
				<xsl:choose>
					<xsl:when test="@Focus='Record' or @Focus='Field'">
						<xsl:text>Plain</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>Hide</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</xsl:element>>
	</xsl:template>
</xsl:stylesheet>