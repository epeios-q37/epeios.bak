<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
 <xsl:output method="html" encoding="UTF-8"/>
 <xsl:template match="/">
  <xsl:apply-templates select="*/Context"/>
 </xsl:template>
 <xsl:template match="Context">
  <xsl:choose>
   <xsl:when test="@Status='Display'">
    <xdh-cast id="AgentDisplaying" kind="Plain"/>
    <xdh-cast id="AgentEditable">
     <xsl:attribute name="kind">
      <xsl:choose>
       <xsl:when test="@Agent">
        <xsl:text>Plain</xsl:text>
       </xsl:when>
       <xsl:otherwise>
        <xsl:text>Hide</xsl:text>
       </xsl:otherwise>
      </xsl:choose>
     </xsl:attribute>
    </xdh-cast>
  </xsl:when>
  <xsl:when test="@Status='Edition'">
   <xdh-cast id="AgentDisplaying" kind="Disable"/>
  </xsl:when>
  <xsl:otherwise>
   <xdh-cast id="AgentDisplaying" kind="Disable"/>
  </xsl:otherwise>
  </xsl:choose>
 </xsl:template>
</xsl:stylesheet>