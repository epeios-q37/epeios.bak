<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/ns/xpp/draft"
				>
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Content"/>
		<xsl:apply-templates select="*/Corpus"/>
	</xsl:template>
	<xsl:template match="Content">
  <span class="vcenter-out">
   <span class="vcenter-in">
    <!--xsl:if test="contains(/RichFrontEnd/@OS,'Linux')">
     <div style="width: 100%; display: flex; justify-content: center;">#LinuxDialogBoxIssue#</div>
    </xsl:if-->
    <fieldset>
     <span id="Agents"/>
    </fieldset>
    <button data-xdh-onevent="CloseConfig">#configClose#</button>
   </span>
  </span>
 </xsl:template>
	<xsl:template match="Corpus">
	</xsl:template>

</xsl:stylesheet>