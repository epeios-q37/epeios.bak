<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns="http://www.w3.org/1999/xhtml">
	<xsl:output method="xml" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xsl:template match="Content">
		<xsl:apply-templates select="About"/>
	</xsl:template>
	<xsl:template match="About">
		<span>
			<xsl:apply-templates select="Frontend"/>
			<br/>
			<xsl:apply-templates select="Software"/>
			<br/>
			<xsl:apply-templates select="Launcher"/>
			<br/>
			<xsl:apply-templates select="Backend"/>
		</span>
	</xsl:template>
	<xsl:template match="Frontend">
		<xsl:value-of select="Name"/>
		<xsl:text> V</xsl:text>
		<xsl:value-of select="Version"/>
		<xsl:text> Build </xsl:text>
		<xsl:value-of select="Build"/>
		<br/>
		<span style="margin-left : 10%">
			<xsl:text>#aBy# </xsl:text>
			<xsl:value-of select="Author/Name"/>
			<xsl:text> (</xsl:text>
			<a target="_blank">
				<xsl:attribute name="href">
					<xsl:value-of select="Author/Contact"/>
				</xsl:attribute>
				<xsl:value-of select="Author/Contact"/>
			</a>
			<xsl:text>)</xsl:text>
		</span>
		<br/>
		<xsl:text>© </xsl:text>
		<xsl:value-of select="Copyright/Years"/>
		<!-- No-break space. '&nbsp' is only available in html, not in xhtml. Under Windows, type Alt+0160-->
		<xsl:text> </xsl:text>
		<xsl:value-of select="Copyright/Owner/Name"/>
		<br/>
	</xsl:template>
	<xsl:template match="Launcher">
		<xsl:text>#aLauncher# :</xsl:text>
		<br/>
		<xsl:value-of select="."/>
		<br/>
	</xsl:template>
	<xsl:template match="Software">
		<div style="text-align : center;">
			<xsl:value-of select="Name"/>
			<xsl:text> V</xsl:text>
			<xsl:value-of select="Version"/>
			<!-- No-break space. '&nbsp' is only available in html, not in xhtml. Under Windows, type Alt+0160-->
			<xsl:text> </xsl:text>
			<a target="_blank">
				<xsl:attribute name="href">http://q37.info/</xsl:attribute>
				<xsl:text>http://q37.info/</xsl:text>
			</a>
		</div>
	</xsl:template>
	<xsl:template match="Backend">
		<xsl:text>#aBackend# (</xsl:text>
		<xsl:value-of select="Plugin"/>
		<xsl:text> : '</xsl:text>
		<xsl:value-of select="PluginParameters"/>
		<xsl:text>') :</xsl:text>
		<br/>
		<xsl:value-of select="Build"/>
		<br/>
	</xsl:template>
</xsl:stylesheet>