<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/ns/xpp/draft"
				>
	<xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xpp:expand href="functions.xsl"/>
	<xsl:template name="DisplayEditableTextEntry">
		<xsl:param name="Type"/>
		<xsl:param name="Content"/>
		<span>
			<xsl:choose>
				<xsl:when test="$Type='Text'">
					<textarea id="EditableEntry" >
						<xsl:value-of select="$Content"/>
					</textarea>
				</xsl:when>
				<xsl:when test="$Type='RichText'">
					<textarea id="EditableEntry">
						<xsl:attribute name="data-xdh-widget">
							<!-- 'xsl:attribute' due to presence of '{' and '}'. -->
							<xsl:text>ckeditor|{enterMode : CKEDITOR.ENTER_BR, linkShowTargetTab: false, language: '#fieldLanguage#', startupFocus : true,}|val\(\)|ckeditor\(\).editor.focus\(\)</xsl:text>
							<!-- Alternative -->
							<!--<xsl:text>jqte</xsl:text>-->
						</xsl:attribute>
						<xsl:value-of select="$Content"/>
					</textarea>
				</xsl:when>
				<xsl:when test="$Type='Date'">
					<input id="EditableEntry">
						<xsl:attribute name="data-xdh-widget">
							<!-- 'xsl:attribute' due to presence of '{' and '}'. -->
							<xsl:text>datepicker|{language:'#fieldLanguage#', autoclose: true, todayHighlight: true, todayBtn: true,}</xsl:text>
						</xsl:attribute>
						<xsl:attribute name="Value">
							<xsl:value-of select="$Content"/>
						</xsl:attribute>
					</input>
				</xsl:when>
				<xsl:when test="$Type='Time'">
					<input id="EditableEntry">
						<xsl:attribute name="data-xdh-widget">
							<!-- 'xsl:attribute' due to presence of '{' and '}'. -->
							<xsl:text>timepicker|{#fieldTimePickerParameters#}</xsl:text>
						</xsl:attribute>
						<xsl:attribute name="Value">
							<xsl:value-of select="$Content"/>
						</xsl:attribute>
					</input>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>#fieldUnknownType#</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</span>
	</xsl:template>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout/Field"/>
	</xsl:template>
	<xsl:template match="Field">
		<xsl:variable name="Type">
			<xsl:call-template name="GetTypeLabel"/>
		</xsl:variable>
		<xsl:variable name="Number">
			<xsl:call-template name="GetNumberLabel"/>
		</xsl:variable>
		<div>
			<xsl:choose>
				<xsl:when test="$Type='Record'">
					<xsl:apply-templates select="Entries" mode="Record">
						<xsl:with-param name="Number" select="$Number"/>
					</xsl:apply-templates>
				</xsl:when>
				<xsl:otherwise>
					<xsl:apply-templates select="Entries">
						<xsl:with-param name="Type" select="$Type"/>
						<xsl:with-param name="Number" select="$Number"/>
					</xsl:apply-templates>
				</xsl:otherwise>
			</xsl:choose>
		</div>
		<xsl:if test="$Number='Multi'">
			<div>
				<button data-xdh-onevents="DefineNewEntry|(drop|DropEntry)" title="#fieldDefineNewEntryTitle#" data-xdh-cast="EntryDroppingCast">#fieldDefineNewEntry#</button>
			</div>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Entries" mode="Record">
		<xsl:param name="Number"/>
		<xsl:text>#fieldNotImplementedYet#</xsl:text>
	</xsl:template>
	<xsl:template match="Entries">
		<xsl:param name="Type"/>
		<xsl:param name="Number"/>
		<xsl:apply-templates select="Entry">
			<xsl:with-param name="Type" select="$Type"/>
			<xsl:with-param name="Number" select="$Number"/>
		</xsl:apply-templates>
		<xsl:if test="not(@Selected) and ($Number!='Mono' or @Amount='0')">
			<xsl:call-template name="DisplayEditableTextEntry">
				<xsl:with-param name="Type" select="$Type"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	<xsl:template match="Entry">
		<xsl:param name="Type"/>
		<xsl:param name="Number"/>
		<xsl:choose>
			<xsl:when test="@id=../@Selected or $Number='Mono'">
				<xsl:call-template name="DisplayEditableTextEntry">
					<xsl:with-param name="Type" select="$Type"/>
					<xsl:with-param name="Content" select="."/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<fieldset title="#fieldDragAndDropTitle#" data-xdh-onevents="(dragstart|DragEntry)|(drop|DropEntry)|(dragend|EndEntryDragging)" data-xdh-casts="EntryDraggingCast|EntryDroppingCast">
					<xsl:attribute name="data-xdh-value">
						<xsl:value-of select="@id"/>
					</xsl:attribute>
					<span data-xdh-onevent="DefineEntry">
						<xsl:attribute name="data-xdh-value">
							<xsl:value-of select="@id"/>
						</xsl:attribute>
						<xsl:call-template name="DOE">
							<xsl:with-param name="Content" select="."/>
						</xsl:call-template>
					</span>
				</fieldset>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>