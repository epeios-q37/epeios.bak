<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
				xmlns="http://www.w3.org/1999/xhtml"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:dpk="http://q37.info/ns/dpk/">
	<xsl:output method="html" indent="yes" encoding="UTF-8"/>
	<xsl:template match="Picking">
		<xsl:element name="head">
			<xsl:element name="title">
				<xsl:text>Instrumentation</xsl:text>
				<xsl:if test="Data/Instrumentations/@Amount&gt;1">
					<xsl:text>s</xsl:text>
				</xsl:if>
			</xsl:element>
		</xsl:element>
		<xsl:element name="body">
			<xsl:apply-templates select="Data"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Data">
		<xsl:apply-templates select="Instrumentations"/>
	</xsl:template>
	<xsl:template match="Instrumentations">
		<xsl:if test="@Amount!=1">
			<xsl:apply-templates select="Instrumentation" mode="Index"/>
			<hr />
		</xsl:if>
		<xsl:apply-templates select="Instrumentation"/>
	</xsl:template>
	<xsl:template match="Instrumentation" mode="Index">
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:text>#</xsl:text>
				<xsl:value-of select="generate-id()"/>
			</xsl:attribute>
			<xsl:value-of select="Name"/>
		</xsl:element>
		<br />
	</xsl:template>
	<xsl:template match="Instrumentation">
		<xsl:element name="a">
			<xsl:attribute name="name">
				<xsl:text>#</xsl:text>
				<xsl:value-of select="generate-id()"/>
			</xsl:attribute>
			<xsl:value-of select="Name"/>
			<xsl:if test="@dpk:label">
				<xsl:text> (</xsl:text>
				<span style="font-style: oblique;">
					<xsl:value-of select="@dpk:label"/>
				</span>
				<xsl:text>)</xsl:text>
			</xsl:if>
		</xsl:element>
		<hr />
	</xsl:template>
</xsl:stylesheet>