<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet	version="1.0"
                xmlns="http://www.w3.org/1999/xhtml"
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
				xmlns:xpp="http://q37.info/xpp"
				>
	<xsl:output method="html" encoding="UTF-8" doctype-system="about:legacy-compat"/>
	<xsl:template match="/">
		<xsl:apply-templates select="*/Layout"/>
	</xsl:template>
	<xsl:template match="Layout">
		<span class="vcenter-out">
			<span class="vcenter-in">
				<div>
					<xsl:apply-templates select="Tabs"/>
				</div>
				<fieldset>
					<span id="Records"/>
					<span id="Record"/>
				</fieldset>
			</span>
		</span>
	</xsl:template>
	<xsl:template match="Tabs">
		<ul class="tab">
			<xsl:apply-templates select="Tab"/>
			<li>
				<a data-xdh-onevents="NewPanel|(drop|DropPanel)" data-xdh-cast="PanelDroppingCast">
					<xsl:text>+</xsl:text>
				</a>
			</li>
		</ul>
	</xsl:template>
	<xsl:template match="Tab">
		<li>
			<a data-xdh-onevent="SelectPanel" data-xdh-onevents="(dragstart|DragPanel)|(drop|DropPanel)|(dragend|EndPanelDragging)" data-xdh-casts="PanelDraggingCast|PanelDroppingCast">
				<xsl:attribute name="data-xdh-value">
					<xsl:value-of select="@id"/>
				</xsl:attribute>
				<xsl:if test="../@Selected=@id">
					<xsl:attribute name="class">active</xsl:attribute>
				</xsl:if>
				<xsl:value-of select="."/>
			</a>
		</li>
	</xsl:template>
</xsl:stylesheet>