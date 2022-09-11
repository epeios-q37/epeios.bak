<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				>
	<xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Casting"/>
	</xsl:template>
	<xsl:template match="Casting">
	</xsl:template>
</xsl:stylesheet>