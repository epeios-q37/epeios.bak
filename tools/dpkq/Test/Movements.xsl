<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:dpk="http://q37.info/ns/dpk/">
	<xsl:output method="html" indent="yes" encoding="UTF-8"/>
	<xsl:template match="Picking">
		<xsl:element name="head">
			<xsl:element name="title">
				<xsl:text>Movement</xsl:text>
				<xsl:if test="Data/Movements/@Amount&gt;1">
					<xsl:text>s</xsl:text>
				</xsl:if>
			</xsl:element>
		</xsl:element>
		<xsl:element name="body">
			<xsl:apply-templates select="Data"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Data">
		<xsl:apply-templates select="Movements"/>
	</xsl:template>
	<xsl:template match="Movements">
		<xsl:if test="@Amount!=1">
			<xsl:apply-templates select="Movement" mode="Index"/>
			<hr />
		</xsl:if>
		<xsl:apply-templates select="Movement"/>
	</xsl:template>
	<xsl:template match="Movement" mode="Index">
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:text>#</xsl:text>
				<xsl:value-of select="generate-id()"/>
			</xsl:attribute>
			<xsl:apply-templates select="Piece" mode="Index"/>
			<xsl:text> - </xsl:text>
			<xsl:element name="b">
				<xsl:value-of select="Title"/>
			</xsl:element>
		</xsl:element>
		<br />
	</xsl:template>
	<xsl:template match="Piece" mode="Index">
		<xsl:apply-templates select="Composer" mode="Index"/>
		<xsl:apply-templates select="Collection" mode="Index"/>
		<xsl:apply-templates select="Number" mode="before"/>
		<xsl:value-of select="Title"/>
		<xsl:apply-templates select="Number" mode="after"/>
		<xsl:apply-templates select="Opus"/>
		<xsl:apply-templates select="SubTitle"/>
	</xsl:template>
	<xsl:template match="Collection" mode="Index">
		<xsl:apply-templates select="Composer" mode="Index"/>
		<xsl:value-of select="Title"/>
		<xsl:text> - </xsl:text>
	</xsl:template>
	<xsl:template match="Composer" mode="Index">
		<em>
			<xsl:value-of select="Surname"/>
			<xsl:text>, </xsl:text>
			<xsl:value-of select="FirstNames"/>
		</em>
		<xsl:text> : </xsl:text>
	</xsl:template>
	<xsl:template match="Movement">
		<xsl:element name="a">
			<xsl:attribute name="name">
				<xsl:text>#</xsl:text>
				<xsl:value-of select="generate-id()"/>
			</xsl:attribute>
			<xsl:apply-templates select="Piece"/>
			<br />
			<xsl:value-of select="Title"/>
			<xsl:if test="@dpk:label">
				<xsl:text> [</xsl:text>
				<span style="font-style: oblique;">
					<xsl:value-of select="@dpk:label"/>
				</span>
				<xsl:text>]</xsl:text>
			</xsl:if>
		</xsl:element>
		<hr />
	</xsl:template>
	<xsl:template match="Piece">
		<xsl:apply-templates select="Instrumentation"/>
		<xsl:apply-templates select="Collection"/>
		<xsl:apply-templates select="Composer"/>
		<xsl:if test="Composer">
			<br />
		</xsl:if>
		<xsl:element name="b">
			<xsl:apply-templates select="Number" mode="before"/>
			<xsl:value-of select="Title"/>
			<xsl:apply-templates select="Number" mode="after"/>
			<xsl:apply-templates select="Opus"/>
			<xsl:apply-templates select="SubTitle"/>
		</xsl:element>
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
		<br />
	</xsl:template>
	<xsl:template match="Collection">
		<xsl:apply-templates select="Instrumentation"/>
		<xsl:apply-templates select="Composer"/>
		<xsl:value-of select="Title"/>
		<xsl:apply-templates select="SubTitle"/>
		<xsl:apply-templates select="Opus"/>
	</xsl:template>
	<xsl:template match="SubTitle">
		<xsl:text> (</xsl:text>
		<xsl:element name="em">
			<xsl:value-of select="."/>
		</xsl:element>
		<xsl:text>)</xsl:text>
	</xsl:template>
	<xsl:template match="Number" mode="before">
		<xsl:if test="@Type='Extern'">
			<xsl:apply-templates select="." mode="extern"/>
			<xsl:text> </xsl:text>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Number" mode="after">
		<xsl:if test="@Type='Embed'">
			<xsl:text> </xsl:text>
			<xsl:apply-templates select="." mode="embed"/>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Number" mode="extern">
		<xsl:value-of select="."/>
		<xsl:text>.</xsl:text>
	</xsl:template>
	<xsl:template match="Number" mode="embed">
		<xsl:text>n°</xsl:text>
		<xsl:value-of select="."/>
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
</xsl:stylesheet>