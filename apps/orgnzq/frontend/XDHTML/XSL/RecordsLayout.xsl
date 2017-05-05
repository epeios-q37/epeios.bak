<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/ns/xpp/draft"
				>
	<xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xpp:expand href="functions.xsl"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
	</xsl:template>
	<xsl:template match="Layout">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<fieldset>
					<xsl:apply-templates select="Records"/>
					<button data-xdh-onevent="DefineNewRecord" title="#recordsDefineNewRecordTitle#">#recordsDefineNewRecord#</button>
				</fieldset>
			</span>
		</span>
	</xsl:template>
	<xsl:template match="Records">
		<fieldset>
			<legend>#recordsLegend#</legend>
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
			<xsl:call-template name="DOE">
				<xsl:with-param name="Content" select="."/>
			</xsl:call-template>
		</fieldset>
	</xsl:template>
</xsl:stylesheet>