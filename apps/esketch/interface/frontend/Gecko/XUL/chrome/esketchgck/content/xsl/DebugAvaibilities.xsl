﻿<?xml version="1.0" encoding="utf-8"?><xsl:stylesheet	version="1.0"                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"                xmlns="http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul">	<xsl:output method="xml"              indent="yes"              encoding="utf-8"              omit-xml-declaration="yes"              standalone="yes"/>	<xsl:template name="hidden">		<xsl:param name="Id"/>		<xsl:param name="Value"/>			<xsl:attribute name="id">				<xsl:value-of select="$Id"/>			</xsl:attribute>			<xsl:attribute name="hidden">				<xsl:value-of select="$Value"/>			</xsl:attribute>	</xsl:template>	<xsl:template name="Hide">		<xsl:param name="Id"/>		<xsl:call-template name="hidden">			<xsl:with-param name="Id" select="$Id"/>			<xsl:with-param name="Value">true</xsl:with-param>		</xsl:call-template>	</xsl:template>	<xsl:template name="Show">		<xsl:param name="Id"/>		<xsl:call-template name="hidden">			<xsl:with-param name="Id" select="$Id"/>			<xsl:with-param name="Value">false</xsl:with-param>		</xsl:call-template>	</xsl:template>	<xsl:template name="disabled">		<xsl:param name="Id"/>		<xsl:param name="Value"/>			<xsl:attribute name="id">				<xsl:value-of select="$Id"/>			</xsl:attribute>			<xsl:attribute name="disabled">				<xsl:value-of select="$Value"/>			</xsl:attribute>	</xsl:template>	<xsl:template name="Disable">		<xsl:param name="Id"/>		<xsl:call-template name="disabled">			<xsl:with-param name="Id" select="$Id"/>			<xsl:with-param name="Value">true</xsl:with-param>		</xsl:call-template>	</xsl:template>	<xsl:template name="Enable">		<xsl:param name="Id"/>		<xsl:call-template name="disabled">			<xsl:with-param name="Id" select="$Id"/>			<xsl:with-param name="Value">false</xsl:with-param>		</xsl:call-template>	</xsl:template>  <xsl:template name="Accessibility">    <xsl:param name="Id"/>    <xsl:param name="Value"/>    <xsl:element name="broadcaster">      <xsl:choose>        <xsl:when test="$Value='Disable'">          <xsl:call-template name="Disable">            <xsl:with-param name="Id" select="$Id"/>          </xsl:call-template>          <xsl:call-template name="Show">            <xsl:with-param name="Id" select="$Id"/>          </xsl:call-template>        </xsl:when>        <xsl:when test="$Value='Hide'">          <xsl:call-template name="Enable">            <xsl:with-param name="Id" select="$Id"/>          </xsl:call-template>          <xsl:call-template name="Hide">            <xsl:with-param name="Id" select="$Id"/>          </xsl:call-template>        </xsl:when>        <xsl:otherwise>          <xsl:call-template name="Enable">            <xsl:with-param name="Id" select="$Id"/>          </xsl:call-template>          <xsl:call-template name="Show">            <xsl:with-param name="Id" select="$Id"/>          </xsl:call-template>        </xsl:otherwise>      </xsl:choose>    </xsl:element>  </xsl:template>	<xsl:template match="/">		<xsl:apply-templates select="Availabilities"/>	</xsl:template>	<xsl:template match="Availabilities">		<xsl:apply-templates select="Epeios"/>	</xsl:template>	<xsl:template match="Epeios">		<xsl:if test="IsConnected='false'">			<xsl:call-template name="Accessibility">				<xsl:with-param name="Id">avbBackendError</xsl:with-param>				<xsl:with-param name="Value">Disable</xsl:with-param>			</xsl:call-template>		</xsl:if>	</xsl:template></xsl:stylesheet>