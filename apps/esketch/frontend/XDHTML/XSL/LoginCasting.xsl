<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="Casting">
		<xsl:apply-templates select="Backend"/>
	</xsl:template>
	<xsl:template match="Backend">
		<xsl:variable name="Type" select="@Type"/>
		<xsl:choose>
			<xsl:when test="$Type='None'">
				<xdh-cast id="NoneBackendCast" kind="Vanish"/>
				<xdh-cast id="StraightBackendCast" kind="Hide"/>
				<xdh-cast id="EmbeddedBackendCast" kind="Hide"/>
				<xdh-cast id="PredefinedBackendCast" kind="Hide"/>
			</xsl:when>
			<xsl:when test="$Type='Straight'">
				<xdh-cast id="NoneBackendCast" kind="Plain"/>
				<xdh-cast id="StraightBackendCast" kind="Plain"/>
				<xdh-cast id="EmbeddedBackendCast" kind="Hide"/>
				<xdh-cast id="PredefinedBackendCast" kind="Hide"/>
			</xsl:when>
			<xsl:when test="$Type='Embedded'">
				<xdh-cast id="NoneBackendCast" kind="Plain"/>
				<xdh-cast id="StraightBackendCast" kind="Hide"/>
				<xdh-cast id="EmbeddedBackendCast" kind="Plain"/>
				<xdh-cast id="PredefinedBackendCast" kind="Hide"/>
			</xsl:when>
			<xsl:when test="$Type='Predefined'">
				<xdh-cast id="NoneBackendCast" kind="Plain"/>
				<xdh-cast id="StraightBackendCast" kind="Hide"/>
				<xdh-cast id="EmbeddedBackendCast" kind="Hide"/>
				<xdh-cast id="PredefinedBackendCast" kind="Plain"/>
			</xsl:when>
		</xsl:choose>
		<xdh-cast id="VisibleBackendCast">
			<xsl:attribute name="kind">
				<xsl:choose>
					<xsl:when test="@Visibility='Show'">
						<xsl:text>Plain</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>Hide</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</xdh-cast>
	</xsl:template>
</xsl:stylesheet>