<?xml version="1.0" encoding="UTF-8"?>
<xpp:bloc
	xmlns:xpp="http://q37.info/ns/xpp/draft"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns="http://www.w3.org/1999/xhtml">
	<xsl:template name="GetNumberId">
		<xsl:choose>
			<xsl:when test="@Number">
				<xsl:value-of select="@Number"/>
			</xsl:when>
			<xsl:when test="local-name()='Number'">
				<xsl:value-of select="@id"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="GetNumberLabel">
		<xsl:param name="Id">
			<xsl:call-template name="GetNumberId"/>
		</xsl:param>
		<xsl:value-of select="//Corpus/Numbers/Number[@id=$Id]/@label"/>
	</xsl:template>
	<xsl:template name="GetNumberWording">
		<xsl:param name="Id">
			<xsl:call-template name="GetNumberId"/>
		</xsl:param>
		<xsl:variable name="Label">
			<xsl:call-template name="GetNumberLabel">
				<xsl:with-param name="Id" select="$Id"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$Label='Mono'">
				<xsl:text>#fMonoNumberWording#</xsl:text>
			</xsl:when>
			<xsl:when test="$Label='Multi'">
				<xsl:text>#fMultiNumberWording#</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>#fUnknownNumber#</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="GetTypeId">
		<xsl:choose>
			<xsl:when test="@Type">
				<xsl:value-of select="@Type"/>
			</xsl:when>
			<xsl:when test="local-name()='Type'">
				<xsl:value-of select="@id"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="GetTypeLabel">
		<xsl:param name="Id">
			<xsl:call-template name="GetTypeId"/>
		</xsl:param>
		<xsl:value-of select="//Corpus/Types/Type[@id=$Id]/@label"/>
	</xsl:template>
	<xsl:template name="GetTypeWording">
		<xsl:param name="Id">
			<xsl:call-template name="GetTypeId"/>
		</xsl:param>
		<xsl:variable name="Label">
			<xsl:call-template name="GetTypeLabel">
				<xsl:with-param name="Id" select="$Id"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$Label='Record'">
				<xsl:text>#fRecordTypeWording#</xsl:text>
			</xsl:when>
			<xsl:when test="$Label='Text'">
				<xsl:text>#fTextTypeWording#</xsl:text>
			</xsl:when>
			<xsl:when test="$Label='RichText'">
				<xsl:text>#fRichTextTypeWording#</xsl:text>
			</xsl:when>
			<xsl:when test="$Label='Date'">
				<xsl:text>#fDateTypeWording#</xsl:text>
			</xsl:when>
			<xsl:when test="$Label='Time'">
				<xsl:text>#fTimeTypeWording#</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>#fUnknownTypeWording#</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="DOE">
		<xsl:param name="Content"/>
		<xsl:value-of select="$Content" disable-output-escaping="yes"/>
	</xsl:template>
</xpp:bloc>