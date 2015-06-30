<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
				xmlns="http://www.w3.org/1999/xhtml"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:dpk="http://q37.info/ns/dpk/">
	<xsl:output method="html" indent="yes" encoding="UTF-8"/>
	<xsl:template match="Picking">
		<xsl:element name="head">
			<xsl:element name="title">
				<xsl:text>Collection</xsl:text>
				<xsl:if test="Data/Collections/@Amount&gt;1">
					<xsl:text>s</xsl:text>
				</xsl:if>
			</xsl:element>
		</xsl:element>
		<xsl:element name="body">
			<xsl:apply-templates select="Data"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Data">
		<xsl:apply-templates select="Collections"/>
	</xsl:template>
	<xsl:template match="Collections">
		<xsl:if test="@Amount!=1">
			<xsl:apply-templates select="Collection" mode="Index"/>
			<hr />
		</xsl:if>
		<xsl:apply-templates select="Collection"/>
	</xsl:template>
	<xsl:template match="Collection" mode="Index">
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:text>#</xsl:text>
				<xsl:value-of select="generate-id()"/>
			</xsl:attribute>
			<xsl:apply-templates select="Composer"/>
			<xsl:value-of select="Title"/>
		</xsl:element>
		<br />
	</xsl:template>
	<xsl:template match="Composer">
		<span style="font-style: oblique;">
			<xsl:value-of select="FirstNames"/>
			<xsl:text> </xsl:text>
			<xsl:value-of select="Surname"/>
		</span>
		<xsl:text> : </xsl:text>
	</xsl:template>
	<xsl:template match="Collection">
		<xsl:element name="a">
			<xsl:attribute name="name">
				<xsl:text>#</xsl:text>
				<xsl:value-of select="generate-id()"/>
			</xsl:attribute>
			<xsl:apply-templates select="Instrumentation"/>
			<br />
			<xsl:apply-templates select="Composer"/>
			<xsl:element name="b">
				<xsl:value-of select="Title"/>
				<xsl:if test="@dpk:label">
					<xsl:text> [</xsl:text>
					<span style="font-style: oblique;">
						<xsl:value-of select="@dpk:label"/>
					</span>
					<xsl:text>]</xsl:text>
				</xsl:if>
				<xsl:apply-templates select="SubTitle"/>
				<xsl:apply-templates select="Opus"/>
			</xsl:element>
		</xsl:element>
		<hr />
	</xsl:template>
	<xsl:template match="Composer">
		<span style="font-style: oblique;">
			<xsl:value-of select="FirstNames"/>
			<xsl:text> </xsl:text>
			<xsl:value-of select="Surname"/>
		</span>
		<xsl:text> : </xsl:text>
	</xsl:template>
	<xsl:template match="Instrumentation">
		<xsl:text> (</xsl:text>
		<xsl:value-of select="Name"/>
		<xsl:text>)</xsl:text>
	</xsl:template>
	<xsl:template match="Opus">
		<xsl:text>, </xsl:text>
		<xsl:element name="em">
			<xsl:choose>
				<xsl:when test="@Catalogue">
					<xsl:value-of select="@Catalogue"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>op.</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> </xsl:text>
			<xsl:value-of select="."/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="SubTitle">
		<xsl:text> (</xsl:text>
		<xsl:value-of select="."/>
		<xsl:text>)</xsl:text>
	</xsl:template>
</xsl:stylesheet>