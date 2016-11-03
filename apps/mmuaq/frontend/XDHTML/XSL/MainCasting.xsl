<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				>
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Context"/>
	</xsl:template>
	<xsl:template match="Context">
		<xsl:choose>
			<xsl:when test="Test/@Enabled='true'">
				<xdh-cast id="TestButtonCasting" kind="Plain"/>
			</xsl:when>
			<xsl:when test="Test/@Enabled='false'">
				<xdh-cast id="TestButtonCasting" kind="Hide"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
