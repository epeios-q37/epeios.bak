<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Context"/>
	</xsl:template>
	<xsl:template match="Context">
  <xdh-cast id="mailCasting">
   <xsl:attribute name="exec">
    <xsl:text>if ( getContent( element ) == '</xsl:text>
    <xsl:value-of select="@CurrentMail"/>
    <xsl:text>') { element.style='cursor: pointer;background-color: lightgrey;';} </xsl:text>
    <xsl:text>else { element.style='cursor: pointer;'; }</xsl:text>
   </xsl:attribute>
  </xdh-cast>
	</xsl:template>
</xsl:stylesheet>