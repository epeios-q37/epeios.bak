<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xpp="http://q37.info/ns/xpp/">
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
		<xsl:apply-templates select="*/Corpus"/>
	</xsl:template>
	<xsl:template match="Layout">
		<xsl:apply-templates select="Fields"/>
	</xsl:template>
	<xsl:template match="Fields">
		<xsl:apply-templates select="Field"/>
	</xsl:template>
	<xsl:template match="Field">
		<fieldset>
			<legend data-xdh-onevent="Edit" style="cursor: pointer;">
				<xsl:attribute name="id">
					<xsl:text>Frame.</xsl:text>
					<xsl:value-of select="@id"/>
				</xsl:attribute>
				<xsl:value-of select="@id"/>
			</legend>
			<xsl:choose>
				<xsl:when test="../@Editable=@id">
					<xsl:choose>
						<xsl:when test="@id='CKEditor'">
							<textarea>
								<xsl:attribute name="id">
									<xsl:value-of select="@id"/>
								</xsl:attribute>
								<xsl:attribute name="data-xdh-widget">
									<xsl:text>ckeditor|{enterMode : CKEDITOR.ENTER_BR, linkShowTargetTab: false, language: '#fieldsLanguage#', startupFocus : true,}|val\(\)|ckeditor\(\).editor.focus\(\)</xsl:text>
									</xsl:attribute>
							</textarea>
						</xsl:when>
						<xsl:when test="@id='JQTE'">
							<textarea data-xdh-widget="jqte" autofocus="true">
								<xsl:attribute name="id">
									<xsl:value-of select="@id"/>
								</xsl:attribute>
							</textarea>
						</xsl:when>
						<xsl:when test="@id='Datepicker'">
							<input>
								<xsl:attribute name="id">
									<xsl:value-of select="@id"/>
								</xsl:attribute>
								<xsl:attribute name="data-xdh-widget">
									<!-- 'xsl:attribute' due to presence of '{' and '}'. -->
									<xsl:text>datepicker|{language:'#fieldsLanguage#', autoclose: true, todayHighlight: true, todayBtn: true,}</xsl:text>
								</xsl:attribute>
							</input>
						</xsl:when>
						<xsl:when test="@id='Timepicker'">
							<input>
								<xsl:attribute name="id">
									<xsl:value-of select="@id"/>
								</xsl:attribute>
								<xsl:attribute name="data-xdh-widget">
									<!-- 'xsl:attribute' due to presence of '{' and '}'. -->
									<xsl:text>timepicker|{#fieldsTimePickerParameters#}</xsl:text>
								</xsl:attribute>
							</input>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<span>
						<xsl:attribute name="id">
							<xsl:value-of select="@id"/>
						</xsl:attribute>
					</span>
				</xsl:otherwise>
			</xsl:choose>
		</fieldset>
	</xsl:template>
	<xsl:template match="Corpus">
	</xsl:template>
</xsl:stylesheet>