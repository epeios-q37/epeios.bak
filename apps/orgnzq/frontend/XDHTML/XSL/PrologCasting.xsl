<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				>
	<xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xsl:template match="Casting">
		<xsl:apply-templates select="ProjectType"/>
	</xsl:template>
	<xsl:template match="ProjectType">
		<xsl:variable name="Type" select="."/>
		<xsl:choose>
			<xsl:when test="$Type='New'">
				<xdh-cast id="NewProjectCast" kind="Vanish"/>
				<xdh-cast id="PredefinedProjectsCast" kind="Hide"/>
				<xdh-cast id="UserProjectCast" kind="Hide"/>
			</xsl:when>
			<xsl:when test="$Type='Predefined'">
				<xdh-cast id="NewProjectCast" kind="Plain"/>
				<xdh-cast id="PredefinedProjectsCast" kind="Plain"/>
				<xdh-cast id="UserProjectCast" kind="Hide"/>
			</xsl:when>
			<xsl:when test="$Type='Remote'">
				<xdh-cast id="NewProjectCast" kind="Plain"/>
				<xdh-cast id="PredefinedProjectsCast" kind="Hide"/>
				<xdh-cast id="UserProjectCast" kind="Plain"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>