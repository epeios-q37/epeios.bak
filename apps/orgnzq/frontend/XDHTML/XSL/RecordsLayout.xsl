<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/ns/xpp/draft"
				>
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Content"/>
	</xsl:template>
	<xsl:template match="Content">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<xsl:apply-templates select="Records"/>
				<button data-xdh-onevent="DefineNewRecord" title="#recordsDefineNewRecordTitle#">#recordsDefineNewRecord#</button>
			</span>
		</span>
	</xsl:template>
	<xsl:template match="Records">
		<fieldset>
			<xsl:if test="@Amount='0'">
				<span style="font-style: italic;">#recordsEmptyList#</span>
			</xsl:if>
			<xsl:apply-templates select="Record"/>
		</fieldset>
	</xsl:template>
	<xsl:template match="Record">
		<fieldset data-xdh-onevent="DefineRecord">
			<xsl:attribute name="data-xdh-value">
				<xsl:value-of select="@id"/>
			</xsl:attribute>
			<xsl:value-of select="."/>
		</fieldset>
	</xsl:template>
</xsl:stylesheet>