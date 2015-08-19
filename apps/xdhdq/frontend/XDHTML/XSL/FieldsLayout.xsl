<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns="http://www.w3.org/1999/xhtml"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/xpp"
				>
	<xsl:output method="html" encoding="UTF-8"/>
	<xsl:template match="/">
		<head>
			<xpp:expand href="styles" />
			<xpp:expand href="keys" />
		</head>
		<body>
			<xsl:apply-templates select="*/Content"/>
			<xpp:expand href="scripts" />
		</body>
	</xsl:template>
	<xsl:template match="Content">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<span>
					<xsl:apply-templates select="Fields"/>
				</span>
			</span>
		</span>
	</xsl:template>
	<xsl:template match="Fields">
		<xsl:apply-templates select="Field"/>
	</xsl:template>
	<xsl:template match="Field">
		<fieldset>
			<legend data-xdh-onevent="Edit" style="cursor: pointer;">
				<xsl:attribute name="id">
					<xsl:value-of select="@id"/>
				</xsl:attribute>
				<xsl:value-of select="@id"/>
			</legend>
			<xsl:choose>
				<xsl:when test="../@Editable=@id">
					<xsl:choose>
						<xsl:when test="@id='CKEditor'">
							<textarea id="EditableEntry" data-xdh-widget="ckeditor|enterMode : CKEDITOR.ENTER_BR, linkShowTargetTab: false, language: '#fieldsLanguage#', startupFocus : true,|val\(\)|ckeditor\(\).editor.focus\(\)">
								<xsl:value-of select="."/>
							</textarea>
						</xsl:when>
						<xsl:when test="@id='JQTE'">
							<textarea id="EditableEntry" data-xdh-widget="jqte" autofocus="true">
								<xsl:value-of select="."/>
							</textarea>
						</xsl:when>
						<xsl:when test="@id='Datepicker'">
							<input id="EditableEntry" data-xdh-widget="datepicker|language:'#fieldsLanguage#', autoclose: true, todayHighlight: true, todayBtn: true,">
								<xsl:attribute name="Value">
									<xsl:value-of select="."/>
								</xsl:attribute>
							</input>
						</xsl:when>
						<xsl:when test="@id='Timepicker'">
							<input id="EditableEntry" data-xdh-widget="timepicker|#fieldsTimePickerParameters#">
								<xsl:attribute name="Value">
									<xsl:value-of select="."/>
								</xsl:attribute>
							</input>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="." disable-output-escaping="yes"/>
				</xsl:otherwise>
			</xsl:choose>
		</fieldset>
	</xsl:template>
</xsl:stylesheet>