<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns="http://www.w3.org/1999/xhtml"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/xpp"
				>
	<xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xsl:template match="/">
	<!--
		<head>
			<xpp:expand href="styles" />
		</head>
		<body data-xdh-onevents="(keypress|About|SC+a)(keypress|Refresh|SC+r)">
			<xsl:apply-templates select="*/Content"/>
		</body>
		-->
		<xsl:apply-templates select="*/Content"/>
	</xsl:template>
	<xsl:template match="Content">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<span id="Records"/>
				<span id="Record"/>
			</span>
		</span>
	</xsl:template>
</xsl:stylesheet>