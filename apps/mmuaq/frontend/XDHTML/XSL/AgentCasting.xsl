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
   <xsl:when test="@Status='Show'">
    <xdh-cast id="InputCast" kind="Disable"/>
   </xsl:when>
   <xsl:when test="@Status='Create' or @Status='Edit'">
    <xdh-cast id="InputCast" kind="Plain"/>
   </xsl:when>
   <xsl:otherwise>
    <xdh-cast id="InputCast" kind="Disable"/>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
</xsl:stylesheet>