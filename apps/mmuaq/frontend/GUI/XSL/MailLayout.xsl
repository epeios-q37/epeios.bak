<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Content"/>
		<xsl:apply-templates select="*/Corpus"/>
	</xsl:template>
	<xsl:template match="Content">
  <div style="white-space: pre-wrap;">
   <xsl:value-of select="Mail"/>
  </div>
	</xsl:template>
	<xsl:template match="Corpus">
	</xsl:template>
 </xsl:stylesheet>