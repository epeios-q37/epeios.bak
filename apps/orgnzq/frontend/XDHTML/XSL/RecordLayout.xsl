<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/ns/xpp/draft"
				>
	<xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
		<xsl:apply-templates select="*/Corpus"/>
	</xsl:template>
	<xsl:template match="Layout">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<fieldset>
					<span id="Fields"/>
					<span id="Column"/>
					<button data-xdh-cast="FieldDefinition" data-xdh-onevent="DefineNewField" title="#recordNewFieldTitle#">#recordNewField#</button>
					<button data-xdh-onevent="BackToList" title="#recordBackToListTitle#">#recordBackToList#</button>
				</fieldset>
			</span>
		</span>
	</xsl:template>
	<xsl:template match="Corpus">
	</xsl:template>
</xsl:stylesheet>