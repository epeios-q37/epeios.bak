<?xml version="1.0" encoding="UTF-8"?>
<!-- NO BOM !! -->
<xsl:stylesheet version="1.0" xmlns="http://www.w3.org/1999/xhtml" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"/>
	<!-- Function -->
	<xsl:template name="GetModeLabel">
		<xsl:param name="Mode" select="@Mode"/>
		<xsl:value-of select="/XDHTML/Corpus/Modes/Mode[@id=$Mode]/@Label"/>
	</xsl:template>
	<!-- End of functions -->
	<xsl:template match="/XDHTML">
		<xsl:apply-templates select="GPIOs"/>
	</xsl:template>
	<xsl:template match="GPIOs">
		<table>
			<thead>
				<tr>
					<th>Id</th>
					<th>Mode</th>
					<th>Value</th>
				</tr>
			</thead>
			<tbody>
				<xsl:apply-templates select="GPIO"/>
			</tbody>
		</table>
	</xsl:template>
	<xsl:template match="GPIO">
		<xsl:variable name="ModeLabel">
			<xsl:call-template name="GetModeLabel"/>
		</xsl:variable>
		<tr>
			<td>
				<xsl:value-of select="@id"/>
			</td>
			<td>
				<select data-xdh-onevent="SwitchMode" id="Mode.{@id}">
					<xsl:apply-templates select="/XDHTML/Corpus/Modes/Mode">
						<xsl:with-param name="Mode" select="@Mode"/>
					</xsl:apply-templates>
				</select>
			</td>
			<td>
				<xsl:element name="input">
					<xsl:attribute name="id">
						<xsl:text>PWM.</xsl:text>
						<xsl:value-of select="@id"/>
					</xsl:attribute>
					<xsl:attribute name="type">range</xsl:attribute>
					<xsl:attribute name="min">0</xsl:attribute>
					<xsl:attribute name="max">100</xsl:attribute>
					<xsl:attribute name="value">
						<xsl:value-of select="@Value"/>
					</xsl:attribute>
					<xsl:choose>
					<xsl:when test="$ModeLabel='IN'">
						<xsl:attribute name="disabled">disabled</xsl:attribute>
					</xsl:when>
					<xsl:when test="$ModeLabel='OUT'">
						<xsl:attribute name="step">100</xsl:attribute>
					</xsl:when>
					</xsl:choose>
				</xsl:element>
			</td>
		</tr>
	</xsl:template>
	<xsl:template match="/XDHTML/Corpus/Modes/Mode">
		<xsl:param name="Mode"/>
		<xsl:element name="option">
			<xsl:attribute name="value">
				<xsl:value-of select="@id"/>
			</xsl:attribute>
			<xsl:if test="$Mode=@id">
				<xsl:attribute name="selected">selected</xsl:attribute>
			</xsl:if>
			<xsl:value-of select="@Label"/>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>