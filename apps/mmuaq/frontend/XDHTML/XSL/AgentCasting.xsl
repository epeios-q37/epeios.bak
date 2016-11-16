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
    <xdh-cast id="EditCast" kind="Disable"/>
    <xdh-cast id="ActionCast" kind="Hide"/>
    <xdh-cast id="PasswordEditionChoiceCast" kind="Hide"/>
    <xdh-cast id="PasswordEditCast" kind="Hide"/>
   </xsl:when>
   <xsl:when test="@Status='Create'">
    <xdh-cast id="EditCast" kind="Plain"/>
    <xdh-cast id="ActionCast" kind="Plain"/>
    <xdh-cast id="PasswordEditionChoiceCast" kind="Hide"/>
    <xdh-cast id="PasswordEditCast" kind="Plain"/>
   </xsl:when>
   <xsl:when test="@Status='Edit'">
    <xdh-cast id="EditCast" kind="Plain"/>
    <xdh-cast id="ActionCast" kind="Plain"/>
    <xdh-cast id="PasswordEditionChoiceCast" kind="Plain"/>
    <xdh-cast id="PasswordEditCast" kind="Plain"/>
   </xsl:when>
   <xsl:otherwise>
    <xdh-cast id="EditCast" kind="Disable"/>
    <xdh-cast id="ActionCast" kind="Hide"/>
    <xdh-cast id="PasswordEditionChoiceCast" kind="Hide"/>
    <xdh-cast id="PasswordEditCast" kind="Hide"/>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
</xsl:stylesheet>