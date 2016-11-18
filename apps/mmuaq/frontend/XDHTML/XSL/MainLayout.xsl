<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns="http://www.w3.org/1999/xhtml"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/xpp"
				>
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Content"/>
	</xsl:template>
	<xsl:template match="Content">
  <span class="vcenter-out">
   <span class="vcenter-in">
    <xsl:apply-templates select="Mails"/>
    <button title="#mainConfigTitle#" data-xdh-onevent="Configuration">#mainConfig#</button>
   </span>
  </span>
 </xsl:template>
 <xsl:template match="Mails">
  <hr/>
  <xsl:apply-templates select="Mail"/>
  <hr/>
 </xsl:template>
 <xsl:template match="Mail">
  <xsl:apply-templates select="@Subject"/>
  <br/>
 </xsl:template>
</xsl:stylesheet>