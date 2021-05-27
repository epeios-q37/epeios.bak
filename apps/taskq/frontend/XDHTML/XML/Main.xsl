<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns="http://www.w3.org/1999/xhtml" 	xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
	</xsl:template>
	<xsl:template match="Layout">
		<textarea id="Input"></textarea>
		<button data-xdh-onevent="Submit">Submit</button>
		<fieldset>
			<output id="Output">Test</output>
		</fieldset>
	</xsl:template>
</xsl:stylesheet>
