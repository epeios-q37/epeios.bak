<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/">
  <xsl:apply-templates select="*/Context"/>
 </xsl:template>
 <xsl:template match="Context">
  <xsl:apply-templates select="Agent"/>
 </xsl:template>
 <xsl:template match="Agent">
  <xsl:choose>
   <xsl:when test="@Status='Show'">
    <xdh-cast id="SelectAgentCast" kind="Plain"/>
    <xdh-cast id="EditAgentCast" kind="Plain"/>
    <xdh-cast id="CreateAgentCast" kind="Plain"/>
   </xsl:when>
   <xsl:when test="@Status='Create' or @Status='Edit'">
    <xdh-cast id="SelectAgentCast" kind="Disable"/>
    <xdh-cast id="EditAgentCast" kind="Disable"/>
    <xdh-cast id="CreateAgentCast" kind="Disable"/>
   </xsl:when>
   <xsl:otherwise>
    <xdh-cast id="SelectAgentCast" kind="Plain"/>
    <xdh-cast id="EditAgentCast" kind="Disable"/>
    <xdh-cast id="CreateAgentCast" kind="Plain"/>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
 </xsl:stylesheet>