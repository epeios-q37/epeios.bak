<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/ns/xpp/draft"
				>
	<xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xpp:expand href="functions.xsl"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
		<xsl:apply-templates select="*/Corpus"/>
	</xsl:template>
	<xsl:template match="Layout">
	</xsl:template>
	<xsl:template match="Corpus">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<fieldset>
					<fieldset>
						<input id="FieldLabel" type="text" placeholder="#columnFieldLabelPlaceholder#" required="required" title="#columnFieldLabelTitle#"/>
						<div style="width: 100%;">
							<xsl:apply-templates select="Types"/>
							<xsl:apply-templates select="Numbers"/>
						</div>
						<textarea id="FieldComment" style="width: 100%;" placeholder="#columnFieldCommentPlaceholder#" title="#columnFieldCommentTitle#"/>
					</fieldset>
					<button class="hcenter" data-xdh-onevent="CreateField" title="#columnCreateFieldTitle#">#columnCreateField#</button>
				</fieldset>
			</span>
		</span>
	</xsl:template>
	<xsl:template match="Types">
		<xsl:element name="select">
			<xsl:attribute name="id">Type</xsl:attribute>
			<xsl:attribute name="size">
				<xsl:value-of select="number(@Amount)-1"/>
			</xsl:attribute>
			<xsl:apply-templates select="Type[@label!='Record']"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Numbers">
		<xsl:element name="select">
			<xsl:attribute name="id">Number</xsl:attribute>
			<xsl:attribute name="style">align: right;</xsl:attribute>
			<xsl:attribute name="size">
				<xsl:value-of select="@Amount"/>
			</xsl:attribute>
			<xsl:apply-templates select="Number"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Type">
		<xsl:element name="option">
			<xsl:attribute name="value">
				<xsl:value-of select="@id"/>
			</xsl:attribute>
			<xsl:call-template name="GetTypeWording"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Number">
		<xsl:element name="option">
			<xsl:attribute name="value">
				<xsl:value-of select="@id"/>
			</xsl:attribute>
			<xsl:call-template name="GetNumberWording"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="Columns">
		<fieldset>
			<xsl:apply-templates select="Column"/>
		</fieldset>
	</xsl:template>
	<xsl:template match="Column">
		<fieldset>
			<xsl:attribute name="title">
				<xsl:value-of select="Comment"/>
			</xsl:attribute>
			<legend data-xdh-onevent="EditField">
				<xsl:value-of select="Label"/>
			</legend>
		</fieldset>
	</xsl:template>
</xsl:stylesheet>