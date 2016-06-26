<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				>
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="Context">
		<xsl:apply-templates select="BackendType"/>
	</xsl:template>
	<xsl:template match="BackendType">
		<xsl:variable name="Type" select="."/>
		<xsl:choose>
			<xsl:when test="$Type='None'">
				<xdh-cast id="NoneBackendCast" kind="Vanish"/>
				<xdh-cast id="RemoteBackendCast" kind="Hide"/>
				<xdh-cast id="EmbeddedBackendCast" kind="Hide"/>
				<xdh-cast id="PredefinedBackendCast" kind="Hide"/>
			</xsl:when>
			<xsl:when test="$Type='Remote'">
				<xdh-cast id="NoneBackendCast" kind="Plain"/>
				<xdh-cast id="RemoteBackendCast" kind="Plain"/>
				<xdh-cast id="EmbeddedBackendCast" kind="Hide"/>
				<xdh-cast id="PredefinedBackendCast" kind="Hide"/>
			</xsl:when>
			<xsl:when test="$Type='Embedded'">
				<xdh-cast id="NoneBackendCast" kind="Plain"/>
				<xdh-cast id="RemoteBackendCast" kind="Hide"/>
				<xdh-cast id="EmbeddedBackendCast" kind="Plain"/>
				<xdh-cast id="PredefinedBackendCast" kind="Hide"/>
			</xsl:when>
			<xsl:when test="$Type='Predefined'">
				<xdh-cast id="NoneBackendCast" kind="Plain"/>
				<xdh-cast id="RemoteBackendCast" kind="Hide"/>
				<xdh-cast id="EmbeddedBackendCast" kind="Hide"/>
				<xdh-cast id="PredefinedBackendCast" kind="Plain"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>